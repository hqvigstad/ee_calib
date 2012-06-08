void runCalib()
{
    TStopwatch timer;
  timer.Start();
  gSystem->Load("libCore.so");
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics.so");
  //____________________________________________________//
  //_____________Setting up required packages___________//
  //____________________________________________________//
  gSystem->Load("libSTEERBase.so");
  gSystem->Load("libSTEER.so");
  gSystem->Load("libESD.so");
  gSystem->Load("libAOD.so");
  gSystem->Load("libANALYSIS.so");
  gSystem->Load("libANALYSISalice.so");
  gROOT->ProcessLine(".include $ALICE_ROOT/include");
  gROOT->ProcessLine(".include $ALICE_ROOT/PHOS");

  gSystem->Load("libMinuit2.so");
  
  //___________Compile analysis task using AClic____________//
  gROOT->LoadMacro("CorrFcn.cxx+g");
  gROOT->LoadMacro("calib.C+g");
  
  calib("../grid/merged-ee-calib.output.root");
}
