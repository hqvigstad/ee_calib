


#include "EqEnCalibTask.h"
#include <TH1F.h>
#include <TH2F.h>
#include <AliPHOSCalibData.h>
#include <AliPHOSGeometry.h>
#include <AliESDEvent.h>
#include <AliPHOSReconstructor.h>

ClassImp(EqEnCalibTask)

EqEnCalibTask::EqEnCalibTask()
: AliAnalysisTaskSE("EqEnCalibTask"),
  fOutput(NULL),
  fEnIM(NULL),
  fAmpIM(NULL),
  fAmpEn(NULL),
  fCorAmpEn(NULL),
  fCalibData(NULL),
  fPhosGeo(NULL),
  fCluArray(NULL),
  fPhosCells(NULL),
  fRun(0),
  fMinCells(3),
  fMinCluE(0.3),
  fMaxDiffRel(0.05),
  fMaxDiffAbs(0.1),
  fMinE(0.),
  fMaxE(10.),
  fNBinsE(1000),
  fMinIM(0.),
  fMaxIM(0.7),
  fNBinsIM(700)
{

}

EqEnCalibTask::EqEnCalibTask(const char* name)
: AliAnalysisTaskSE(name),
  fOutput(NULL),
  fEnIM(NULL),
  fAmpIM(NULL),
  fAmpEn(NULL),
  fCorAmpEn(NULL),
  fCalibData(NULL),
  fPhosGeo(NULL),
  fCluArray(NULL),
  fPhosCells(NULL),
  fRun(0),
  fMinCells(3),
  fMinCluE(0.3),
  fMaxDiffRel(0.05),
  fMaxDiffAbs(0.1),
  fMinE(0.),
  fMaxE(10.),
  fNBinsE(1000),
  fMinIM(0.),
  fMaxIM(0.7),
  fNBinsIM(700)
{
  DefineOutput(1, TList::Class());
}



EqEnCalibTask::~EqEnCalibTask()
{
  delete fCluArray;
}


void EqEnCalibTask::UserCreateOutputObjects()
{
  AliAnalysisTaskSE::UserCreateOutputObjects();

  fOutput = new TList;
  fOutput->SetOwner();
  
  fEnIM = new TH2F("fEnIM", "Amplitude IM", fNBinsE, fMinE, fMaxE, fNBinsIM, fMinIM, fMaxIM);
  fEnIM->GetXaxis()->SetTitle("A (Un-corrected Energy) [GeV]");
  fEnIM->GetYaxis()->SetTitle("M *A_i/E_i *A_j/E_j [GeV/c^2] (Un-Corrected IM)");
  fOutput->Add(fEnIM);

  fAmpIM = new TH2F("fAmpIM", "Un-corrected IM", fNBinsE, fMinE, fMaxE, fNBinsIM, fMinIM, fMaxIM);
  fAmpIM->GetXaxis()->SetTitle("A [GeV] (Un-corrected Energy)");
  fAmpIM->GetYaxis()->SetTitle("M [GeV/c^2]");
  fOutput->Add(fAmpIM);

  fAmpEn = new TH1F("fAmpEn", "Amplitude over Energy", 1000, 0, 2);
  fAmpEn->GetXaxis()->SetTitle("A/E");
  fOutput->Add(fAmpEn);

  fEnAmp = new TH2F("fEnAmp", "MC Energy over Amplitude", fNBinsE, fMinE, fMaxE, 1000, 0, 2);
  fEnAmp->GetXaxis()->SetTitle("Amplitude");
  fEnAmp->GetYaxis()->SetTitle("'MC E'/A");

  fGammaPID = new TH1F("fGammaPID", "Photon/Gamma PID", 1001, 0, 1.001);
  fGammaPID->GetXaxis()->SetTitle("kGamma PID");

  fCorAmpEn = new TH1F("fCorAmpEn", "Corrected Un-Corrected over Corrected", 1000, 0, 2);
  fCorAmpEn->GetXaxis()->SetTitle("C^{(sim)}(A)/E");
  fOutput->Add(fCorAmpEn);
  

  fCalibData = new AliPHOSCalibData();
  fPhosGeo =  AliPHOSGeometry::GetInstance("IHEP");

  PostData(1, fOutput);
}


Bool_t EqEnCalibTask::UserNotify()
{
  Printf("notify!");
  return AliAnalysisTaskSE::UserNotify();
}


void EqEnCalibTask::UserExec(Option_t* option)
{
  AliAnalysisTaskSE::UserExec(option);

  const AliVEvent* event = InputEvent();
  const AliMCEvent* mcEvent = MCEvent();
  const AliESDEvent* esd = dynamic_cast<const AliESDEvent*>(event);
//   if( ! esd )
//     Fatal("UserExec", "event not castable to ESD, EqEnCalibTask is only incompatible with ESD");
  
  fRun = event->GetRunNumber();
  fPhosCells = event->GetPHOSCells();
  
  Double_t vtx[3]; //vertex
  event->GetPrimaryVertex()->GetXYZ(vtx);
  const TVector3 vertex(vtx);
  if( DebugLevel() >= 3 )
    Printf("vertex: x:%f y:%f z:%f", vtx[0], vtx[1], vtx[2]);

  if( ! fCluArray )
    fCluArray = new TRefArray;
  event->GetPHOSClusters(fCluArray);
  const Int_t nClusters = fCluArray->GetEntriesFast(); // assumes "no gaps"

  for(int ci1 = 0; ci1 < nClusters; ++ci1) {
    AliVCluster* cluster1 = PassCluster( dynamic_cast<AliVCluster*> ( fCluArray->At(ci1) ) );
    if( ! cluster1 )
      continue;
    const double unCorrectedEnergy1 = GetUCEnergy(cluster1);
    const double fraction1 = unCorrectedEnergy1 / cluster1->E();
    fAmpEn->Fill( fraction1 );
    double corUnCor1 = AliPHOSReconstructor::CorrectNonlinearity(unCorrectedEnergy1);
    fCorAmpEn->Fill( corUnCor1 / cluster1->E() );
    
    for(int ci2 = ci1+1; ci2 < nClusters; ++ci2) {
      AliVCluster* cluster2 = PassCluster( dynamic_cast<AliVCluster*> ( fCluArray->At(ci2) ) );
      if( ! cluster2 )
	continue;
      
      const double unCorrectedEnergy2 = GetUCEnergy(cluster2);
      const double fraction2 = unCorrectedEnergy2 / cluster2->E();
      const double absDiff = TMath::Abs(unCorrectedEnergy1 - unCorrectedEnergy2);
      const double relDiff = absDiff / unCorrectedEnergy1;

      if( absDiff > fMaxDiffAbs )
	continue;
      if( relDiff > fMaxDiffRel )
	continue;

      TLorentzVector p1, p2, p12;
      cluster1->GetMomentum(p1, vtx);
      cluster2->GetMomentum(p2, vtx);
      p12 = p1 + p2;
      if( DebugLevel() >= 5 )
	Printf("IM: %f", p12.M());
      const double unCorrectedIM = p12.M() * fraction1 * fraction2;
      
      fEnIM->Fill(unCorrectedEnergy1, p12.M());
      fAmpIM->Fill(unCorrectedEnergy1, unCorrectedIM );
    }
  }

  PostData(1, fOutput);
}

void EqEnCalibTask::Terminate(Option_t* )
{

}

AliVCluster* EqEnCalibTask::PassCluster(AliVCluster* cluster)
{
  if( ! cluster )
    return NULL;
  if( cluster->GetNCells() < fMinCells )
    return NULL;
//   if( cluster->E() < fMinCluE );
//     return NULL;
  return cluster;
}

double EqEnCalibTask::GetUCEnergy(AliVCluster* cluster)
{
  const int nCells = cluster->GetNCells();
  double energySum = 0.;
  for( int cell = 0; cell < nCells; ++cell) {
    UShort_t absId = cluster->GetCellAbsId( cell );
    double fraction = cluster->GetCellAmplitudeFraction( cell );
    double amplitude = fPhosCells->GetCellAmplitude(absId);

    energySum += fraction * amplitude;
  }
  return energySum;
}
