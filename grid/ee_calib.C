void ee_calib(const char* dataset="")
{
  TStopwatch timer;
  timer.Start();

  gSystem->Load("libCore.so");
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics.so");
  
  //load analysis framework
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice"); //AliAnalysisTaskSE
  
  gSystem->AddIncludePath("-I$ALICE_ROOT/include -I$ALICE_ROOT/PHOS");

  // A task can be compiled dynamically with AClic
  gROOT->LoadMacro("EqEnCalibTask.cxx+g");
  
  // Connect to alien
  TString token = gSystem->Getenv("GRID_TOKEN") ;
  if ( token == "OK" ) 
    TGrid::Connect("alien://");
  else 
    AliInfo("You are not connected to the GRID") ; 

  cout << "EqEnCalib:processing collection " << dataset << endl;
  
  // Create the chain
  TChain* chain = new TChain("esdTree");
  TGridCollection * collection = dynamic_cast<TGridCollection*>(TAlienCollection::Open(dataset));
  
  TAlienResult* result = collection->GetGridResult("",0 ,0);
  TList* rawFileList = result->GetFileInfoList();
  
  for (Int_t counter=0 ; counter < rawFileList->GetEntries() ; counter++) {
    TFileInfo * fi =  static_cast<TFileInfo*>(rawFileList->At(counter)) ; 
    const char * rawFile = fi->GetCurrentUrl()->GetUrl() ;  
    printf("Processing %s\n", rawFile) ;
    chain->Add(rawFile);
    printf("Chain: %d entries.\n",chain->GetEntries()); 
  }
  
  // Make the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("EqEnCalib");
  
  // ESD input handler
  AliESDInputHandler* esdH = new AliESDInputHandler();
  esdH->SetReadFriends(kFALSE);
  mgr->SetInputEventHandler(esdH);
  
  // Debug level
  mgr->SetDebugLevel(0);

  // Add physics selection
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
  AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection();

  // Add my task
  EqEnCalibTask *task = new EqEnCalibTask("EqEnCalibTask");

  task->SelectCollisionCandidates(AliVEvent::kPHI7);
  mgr->AddTask(task);
  
  // Create containers for input/output
  AliAnalysisDataContainer *cinput   = mgr->GetCommonInputContainer(); 
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("outContainer",TList::Class(),AliAnalysisManager::kOutputContainer,"ee-calib.output.root");
  
  // Connect input/output
  mgr->ConnectInput(task , 0, cinput);
  mgr->ConnectOutput(task, 1, coutput1);
  
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("local", chain);
  }
}
