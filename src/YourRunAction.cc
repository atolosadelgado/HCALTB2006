#include "YourRunAction.hh"

#include "YourPrimaryGenerator.hh"
#include "YourEventAction.hh"
#include "YourInputArgs.hh"

#include <iostream>

#include "G4AnalysisManager.hh"
#include "G4Material.hh"



YourRunAction::YourRunAction(const YourInputArgs * args):
          G4UserRunAction(),
           fInputArgs(args)
          {
            // to make UI commands available
            auto analysisManager = G4AnalysisManager::Instance();
}


YourRunAction::~YourRunAction() {}

#include "G4RunManager.hh"
#include "G4VUserActionInitialization.hh"
void YourRunAction::BeginOfRunAction(const G4Run*)
{
    this->ConstructOutputTree();
    this->BeginOutputTree();
}

void YourRunAction::EndOfRunAction(const G4Run* ){
    this->EndOutputTree();
    if (G4Threading::IsMasterThread()){
        fInputArgs->SaveToROOTfile( G4AnalysisManager::Instance()->GetFileName());
    }

}

void YourRunAction::ConstructOutputTree()
{
  auto analysisManager = G4AnalysisManager::Instance();
//   analysisManager->SetDefaultFileType("root"); // set in macrofile
  analysisManager->SetVerboseLevel(1);
  // just to avoid a warning
  if(1<fInputArgs->nthreads)
      analysisManager->SetNtupleMerging(true);  // important for MT

  analysisManager->CreateNtuple("tree", "tree for HCAL 2006 TB experiment");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse");
  analysisManager->FinishNtuple();
}

void YourRunAction::BeginOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    std::string ofilename = analysisManager->GetFileName();
    const std::string phys = fInputArgs->physics_list;
    const std::string birk = std::to_string(fInputArgs->saturation);

    // check position of '.'
    std::size_t dotPos = ofilename.find_last_of('.');

    if (std::string::npos != dotPos ) {
        // if it has extension
        std::string basename  = ofilename.substr(0, dotPos);
        std::string extension = ofilename.substr(dotPos); // including '.'

        ofilename = basename + "_" + phys + "_birk" + birk + extension;
    } else {
        // if no extension
        ofilename = ofilename + "_" + phys + "_birk" + birk;
    }
    analysisManager->SetFileName(ofilename);
    analysisManager->OpenFile(); // name set in macrofile
}

void YourRunAction::EndOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}
