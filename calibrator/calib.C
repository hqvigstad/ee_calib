#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include <TAxis.h>
#include "TMath.h"
#include <TH1F.h>
#include "TPaveText.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/VariableMetricMinimizer.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnMigrad.h"
using namespace ROOT::Minuit2;



#include "CorrFcn.h"
#include "Henrik2010.h"

#include <iostream>
#include <vector>

using namespace std;


CorrFcn createCorrFcn(TH2F* hist);


void calib(const TString& fileName ="ee-calib.output.root",
	   const TString& listName = "outList")
{
  TFile* file = new TFile(fileName, "OPEN");

  TList* list = dynamic_cast<TList*> (file->Get("outContainer") );
  if( ! list ) {
    Printf("No list \"%s\" was found in file %s", listName.Data(), fileName.Data());
    return;
  }
  TH2F* unCorrectedIM = dynamic_cast<TH2F*>( list->FindObject("fUnCorrectedIM") );
  if( ! unCorrectedIM ) {
    Printf("\"fUnCorrectedIM\" was not found in list %s, returning", listName.Data());
    return;
  }
  

  Henrik2010 fnc = createCorrFcn(unCorrectedIM);
  MnUserParameters iParams = fnc.GetInitParameters();
  MnMigrad migrad(fnc, iParams);
  
  FunctionMinimum min = migrad(10000);
  cout << min << endl;
  
  const vector<double>& minParams = min.UserParameters().Params();
  TF1 newFunc = fnc.CreateF(minParams);
  TGraphErrors newCorrGraph = fnc.CreateIMGraph(minParams);

  new TCanvas;
  newFunc.SetNpx(10000);
  newFunc.DrawCopy("");
  TF1 initFunc = fnc.CreateF(iParams.Params());
  initFunc.SetLineColor(kRed);
  initFunc.DrawCopy("same");
  
  new TCanvas;
  unCorrectedIM->DrawCopy("colz");
  newCorrGraph.DrawClone("sampP");
  TF1 I = TF1("I", "0.1349766", 0, 5);
  I.DrawCopy("same");

  TPaveText *pt = new TPaveText(.78, .85, .98, .98, "ndc");
  char txt[256];
  sprintf(txt, "#frac{#chi^{2}}{ndf} = #frac{%f}{%d}", min.Fval(), fnc.GetNPoints() - min.Parameters().Vec().size());
  pt->AddText(txt);
  pt->Draw();

    
}

CorrFcn createCorrFcn(TH2F* hist)
{
  Double_t startE = 1.;
  Double_t startBinWidth = 0.249999999999999;
  Double_t binWidthIncrement = 0.249999999999999;
  int minEntries = 100;
  double maxMeanError = 0.1;

  TCanvas* fitPlotsCanv =0;
  const int nPads = 4*3;
  int currentPad = 0;


  TAxis* xAxis = hist->GetXaxis();
  Int_t nBins = xAxis->GetNbins();
  double* lowEdges = new Double_t[nBins+2];
  xAxis->GetLowEdge(lowEdges);
  double lowE = startE;
  double upE = startE + startBinWidth;
  int nSuccess = 0;
  double* energies = new Double_t[nBins];
  double* energies_width = new Double_t[nBins];
  double* means = new Double_t[nBins];
  double* mean_errors = new Double_t[nBins];
  // Function to be fitted:
  TF1* fitFunc = new TF1("func", "gaus(0)+pol1(3)", 0.07, 0.2);
  //TF1* func = new TF1("func", "gaus(0)", 0.1, 0.15);
  while ( upE < xAxis->GetBinUpEdge(nBins) ) //loop over projection bins
  {
    // Project range of E of 2d Histogram to 1D.
    int lowBin = TMath::BinarySearch<Double_t>(nBins, lowEdges, lowE) +1;
    int upBin = TMath::BinarySearch<Double_t>(nBins, lowEdges, upE) +1;
    lowE = xAxis->GetBinLowEdge(lowBin);
    upE = xAxis->GetBinUpEdge(upBin);

    // Do pojection and do basic verification
    Printf("projecting from: %f (bin %d) to: %f (bin %d)", lowE, lowBin, upE, upBin);
    TH1D* projY = hist->ProjectionY("projY", lowBin, upBin, "e");
    int nEntries = projY->GetEntries();
    if( nEntries < minEntries ) {
      cout << "projection has to few entries " << nEntries << ", widening range" << endl;
      // increase range
      upE += binWidthIncrement;
      continue;
    }
    // set name/title
    char name[256];
    sprintf(name, "proj_%1.2f_%1.2f", lowE, upE);
    projY->SetName(name);
    char title[256];
    sprintf(title, "Un-Corrected IM, %1.1f < p_T < %1.1f", lowE, upE );
    projY->SetTitle(title);
    projY->GetXaxis()->SetRangeUser(0., 0.3);

    // Prepare canvas
    if( !fitPlotsCanv || nPads == currentPad ){
      fitPlotsCanv = new TCanvas;
      fitPlotsCanv->DivideSquare(nPads);
      currentPad = 0;
    }
    fitPlotsCanv->cd(++currentPad);

    // Do Fit, automatically  drawn on canvas:
    fitFunc->SetParameters(projY->GetMaximum(), 0.12, 0.003, 0., 0.);
    TFitResultPtr r = projY->Fit(fitFunc, "RLS", "");

    // Test Fit:
    if( (int) r)
    { // if error
      TPaveText *pt = new TPaveText(.6, .4, .8, .6, "ndc");
      pt->SetTextColor(kRed);
      pt->AddText("Rejected");
      pt->AddText("Fit Error"); 
      pt->Draw();
    }
    else if ( r->ParError(1) > maxMeanError ) {
      TPaveText *pt = new TPaveText(.6, .4, .8, .6, "ndc");
      pt->SetTextColor(kRed);
      pt->AddText("Rejected");
      pt->AddText("Fit Error");
      pt->Draw();
    }
    else //, success
    {
      energies[nSuccess] = (upE + lowE)/2;
      energies_width[nSuccess] = (upE - lowE)/2/*/TMath::Sqrt(12.)*/;
      means[nSuccess] = r->Parameter(1);
      mean_errors[nSuccess] = r->ParError(1);
      cout << nSuccess << " "<< means[nSuccess];
      nSuccess++;
    }

    // set new energy ranges for next projection
    lowE = lowEdges[(upBin-1) + 1];
    upE =lowE + startBinWidth;
    cout << endl;
  }

  new TCanvas;
  TGraphErrors* graph = new TGraphErrors(nSuccess, energies, means, energies_width, mean_errors);
  //TH2F* graphHist = graphHist
  hist->DrawCopy("colz");
  graph->Draw("sameP");
  TF1 I = TF1("I", "0.1349766", 0, 5);
  I.DrawCopy("same");



  CorrFcn fnc = CorrFcn(nSuccess, means, energies, mean_errors, energies_width );
  
  delete lowEdges;
  delete energies;
  delete energies_width;
  delete means;
  delete mean_errors;
  delete fitFunc;

  cout << "FNC has been created with " << nSuccess << " data-points" << endl << endl;
  return fnc;
}
