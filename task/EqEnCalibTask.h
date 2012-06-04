#ifndef ETATASK_H
#define ETATASK_H

class AliPHOSCalibData;
class AliPHOSGeometry;
class TH2F;
class TList;
class AliVEvent;
class TRefArray;
class AliVVertex;
class AliVCaloCells;
class EtaHandler;
class TH1F;
class TH1;

#include <string>


#include "AliAnalysisTaskSE.h"

class EqEnCalibTask : public AliAnalysisTaskSE
{
 public:
  EqEnCalibTask();
  EqEnCalibTask(const char* name);
  virtual ~EqEnCalibTask();

  virtual void UserCreateOutputObjects();
  virtual Bool_t UserNotify();
  virtual void UserExec(Option_t * );
  virtual void Terminate(Option_t *);

 protected:
  EqEnCalibTask(const EqEnCalibTask&);//Not Implemented
  EqEnCalibTask& operator=(const EqEnCalibTask& );//Not Implemented

  AliVCluster* PassCluster(AliVCluster* cluster);
  //AliVCluster* PassClusterPair(const AliVCluster* seed, const AliVCluster* other_return);
  double GetUCEnergy(AliVCluster* cluster);
  
  TList* fOutput;

  TH2F* fCorrectedIM;
  TH2F* fUnCorrectedIM;
  TH1F* fUnCorCor;
  TH1F* fCorUnCorCor;

  AliPHOSCalibData* fCalibData;
  AliPHOSGeometry* fPhosGeo;
  TRefArray* fCluArray; //!
  AliVCaloCells* fPhosCells;

  Int_t fRun;
  
  // cut parameters
  Int_t fMinCells;
  Double_t fMinCluE;
  Double_t fMaxDiffRel;
  Double_t fMaxDiffAbs;
  
  // range parameters
  Double_t fMinE;
  Double_t fMaxE;
  UInt_t fNBinsE;
  Double_t fMinIM;
  Double_t fMaxIM;
  Double_t fNBinsIM;

  ClassDef(EqEnCalibTask, 1);
};

#endif

