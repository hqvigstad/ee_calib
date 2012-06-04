void runLocal() {
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

  //___________Compile analysis task using AClic____________//
  gROOT->LoadMacro("EqEnCalibTask.cxx+g");

  // chain using tags
//   AliTagAnalysis *tagAna = new AliTagAnalysis("ESD");
//   tagAna->ChainLocalTags("tags");
// 
//   AliRunTagCuts *runCuts = new AliRunTagCuts();
//   AliLHCTagCuts *lhcCuts = new AliLHCTagCuts();
//   AliDetectorTagCuts *detCuts = new AliDetectorTagCuts();
//   AliEventTagCuts *evCuts = new AliEventTagCuts();
//   // evCuts->SetMultiplicityRange(11,12);
//   TChain* chain = 0x0;
//   chain = tagAna->QueryTags(runCuts, lhcCuts, detCuts, evCuts);

  // chain using list of files.
  gROOT->LoadMacro("$ALICE_ROOT/PWGUD/macros/CreateESDChain.C");
  TChain* chain = CreateESDChain("files.txt", 10);

  
  // Make the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("TestManager");
  AliESDInputHandler* esdH = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdH);
  AliAnalysisDataContainer *cinput0 = mgr->GetCommonInputContainer();
  
  // Create the task
  EqEnCalibTask *task = new EqEnCalibTask("EqEnCalibTask");
  //task.SetDebugLevel(4);
  mgr->AddTask(task);
  mgr->ConnectInput(task, 0, cinput0);
  AliAnalysisDataContainer *outContainer = mgr->CreateContainer("outContainer", TList::Class(),AliAnalysisManager::kOutputContainer, "ee-calib.output.root");
  mgr->ConnectOutput(task, 1, outContainer);
  
  // Start the task
  mgr->SetDebugLevel(1);
  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis("local",chain);

  timer.Stop();
  timer.Print();
}
