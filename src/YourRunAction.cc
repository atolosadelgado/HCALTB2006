#include "YourRunAction.hh"

#include "YourInputArgs.hh"
#include "YourPrimaryGenerator.hh"
#include "YourEventAction.hh"

#include <iostream>

#include "G4AnalysisManager.hh"




YourRunAction::YourRunAction(const YourInputArgs * args):
          G4UserRunAction(),
          fOutputFileName(args->outputFileName),
          fInputArgs(args)
          {
            // to make UI commands available
            auto analysisManager = G4AnalysisManager::Instance();
}


YourRunAction::~YourRunAction() {}


void YourRunAction::BeginOfRunAction(const G4Run*)
{
    this->BeginOutputTree();
}

void YourRunAction::EndOfRunAction(const G4Run* ){
    this->EndOutputTree();
#if HAVE_ROOT
    if (G4Threading::IsMasterThread()){
        fInputArgs->SaveToROOTfile( G4AnalysisManager::Instance()->GetFileName());
    }
#endif

}


void YourRunAction::BeginOutputTree()
{
  auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetDefaultFileType("root"); // set in macrofile
  analysisManager->SetVerboseLevel(1);

  // just to avoid a warning from G4Analysis
  if(1<fInputArgs->nthreads)
      analysisManager->SetNtupleMerging(true);  // important for MT

  analysisManager->CreateNtuple("tree", "tree for HCAL 2006 TB experiment");
  G4int id = -1;
  id = analysisManager->CreateNtupleDColumn("ECAL_eresponse");
  fEventAction->SetIdNtuple_EcalEnergy(id);
  analysisManager->CreateNtupleDColumn("HCAL_eresponse");
  fEventAction->SetIdNtuple_HcalEnergy(id);
  analysisManager->FinishNtuple();

  // if user did not provide an output file name, create one
  if(fOutputFileName.empty())
  {
      fOutputFileName = analysisManager->GetFileName();
    const std::string phys = fInputArgs->physics_list;

    // check position of '.'
    std::size_t dotPos = fOutputFileName.find_last_of('.');

    if (std::string::npos != dotPos ) {
        // if it has extension
        std::string basename  = fOutputFileName.substr(0, dotPos);
        std::string extension = fOutputFileName.substr(dotPos); // including '.'

        fOutputFileName = basename + "_" + phys + extension;
    } else {
        // if no extension
        fOutputFileName = fOutputFileName + "_" + phys;
    }
  }
  analysisManager->SetFileName(fOutputFileName);
  analysisManager->OpenFile(); // name set in macrofile
}

void YourRunAction::EndOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}
