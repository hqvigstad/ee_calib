/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */


#include "CorrFcn.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <TAxis.h>
#include <Rtypes.h>
using namespace std;
using namespace ROOT::Minuit2;

CorrFcn::CorrFcn(int nSamples, double* peakPos, double* ampp, double* peakError, double* ampw)
: peak(peakPos, peakPos+nSamples),
  peakErr(peakError, peakError+nSamples),
  amp(ampp, ampp+nSamples),
  ampWidth(ampw, ampw+nSamples),
  errorDef(1.)
{
//   for(unsigned int i = 0; i<(unsigned int)nSamples; ++i)
//     cout << peakPos[i] << endl;
}

CorrFcn::CorrFcn(const CorrFcn& other)
: peak(other.peak), peakErr(other.peakErr), amp(other.amp), ampWidth(other.ampWidth), errorDef(other.errorDef)
{;}

/* This function that represents the power function to be minimized, i.e. the X^2
 * @p parameters of the 
 */
inline double CorrFcn::operator()(const std::vector<double>& p) const
{
  double chiSquare = 0.;
  for(unsigned int i = 0; i < peak.size(); ++i) {
    const double factor = f(amp[i],p);
    const double diffWeighted = (factor*factor*peak[i] - I(amp[i])) / (factor*factor*peakErr[i]);
    chiSquare += diffWeighted*diffWeighted;
  }
  return chiSquare;
}

ROOT::Minuit2::MnUserParameters CorrFcn::GetInitParameters()
{
  double ipars[3]={0.0241, 1.0504, 0.000249};
  vector<double> iparameters = vector<double>(ipars, ipars+3);
  double ipar_errs[3]={0.000241, 0.0010504, 0.00000249};
  vector<double> iparameter_errs = vector<double>(ipar_errs, ipar_errs+3);
  
  MnUserParameters params = MnUserParameters(iparameters, iparameter_errs);
  return params;
}

double CorrFcn::f(const double& A, const std::vector<double>& p) const
{
  return p[0]+p[1]*A +p[2]*A*A;
}

TF1 CorrFcn::CreateF(const std::vector< double >& par) const
{
  TF1 func = TF1("f", "[0]+[1]*x + [2]*x*x", 0, 10) ;
  func.SetParameters(&par[0]);
  func.GetXaxis()->SetTitle("A [APD]");
  func.GetYaxis()->SetTitle("E [GeV]");
  return func;
}



TGraphErrors CorrFcn::CreateIMGraph(const std::vector< double >& par) const
{
  unsigned int N = peak.size();
  const double* x = &amp[0];
  double* y = new Double_t[N];
  const double* xe = &ampWidth[0];
  double* ye = new Double_t[N];
  

  for(unsigned int i = 0; i<N; ++i ) {
    y[i] = f(x[i], par)*f(x[i], par) *peak[i];
    ye[i] = f(x[i], par)*f(x[i], par) *peakErr[i]; // TODO correct this
  }
  
  TGraphErrors graph = TGraphErrors(N, x, y, xe, ye);
  return graph;
}
