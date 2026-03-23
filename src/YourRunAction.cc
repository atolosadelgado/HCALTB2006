#include "YourRunAction.hh"

#include "YourInputArgs.hh"
#include "YourPrimaryGenerator.hh"
#include "YourEventAction.hh"
#include "YourTrackingAction.hh"
#include "YourCaloSD.hh"
#include "YourDetectorConstructor.hh"
#include "YourParticleInfo.hh"

#include <iostream>

#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4PhysicsModelCatalog.hh"

YourRunAction::YourRunAction(const YourInputArgs * args, const YourDetectorConstructor * detector):
          G4UserRunAction(),
          fOutputFileName(args->outputFileName),
          fInputArgs(args),
          fDetector(detector)
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
  auto SDmanager = G4SDManager::GetSDMpointer();
  auto* ecalSD = dynamic_cast<YourCaloSD*>(SDmanager->FindSensitiveDetector(fDetector->GetEcalSDname() ));
  auto* hcalSD = dynamic_cast<YourCaloSD*>(SDmanager->FindSensitiveDetector(fDetector->GetHcalSDname() ));

  auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetDefaultFileType("root"); // set in macrofile
  analysisManager->SetVerboseLevel(1);

  // just to avoid a warning from G4Analysis
  if(1<fInputArgs->nthreads)
      analysisManager->SetNtupleMerging(true);  // important for MT

  analysisManager->CreateNtuple("tree", "tree for HCAL 2006 TB experiment");
  G4int id = -1;

  id = analysisManager->CreateNtupleDColumn("ECAL_eresponse");
  ecalSD->SetNTupleColumnID(id);
  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw");

  id = analysisManager->CreateNtupleDColumn("HCAL_eresponse");
  hcalSD->SetNTupleColumnID(id);
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw");

  analysisManager->FinishNtuple();

  // create histograms
  {
      int nmodels = G4PhysicsModelCatalog::Entries();
      YourParticleInfoMap particleInfoMap;
      // e-, pdg 11
      {
          YourParticleInfo electronInfo;
          electronInfo.pdg=11;
          electronInfo.hIDe0 = analysisManager->CreateH2("hE0_electron","",1000,-6,6, nmodels,0,nmodels);
          electronInfo.hIDef = analysisManager->CreateH2("hEf_electron","",1000,-6,6, nmodels,0,nmodels);
          electronInfo.hIDtf = analysisManager->CreateH2("hTf_electron","",1000,-6,6, nmodels,0,nmodels);
          particleInfoMap.emplace( electronInfo.pdg, electronInfo );
      }

      // other particles
      {
          YourParticleInfo electronInfo;
          electronInfo.pdg=YourParticleInfo::PDG_OTHER;
          electronInfo.hIDe0 = analysisManager->CreateH2("hE0_other","",1000,-6,6, nmodels,0,nmodels);
          electronInfo.hIDef = analysisManager->CreateH2("hEf_other","",1000,-6,6, nmodels,0,nmodels);
          electronInfo.hIDtf = analysisManager->CreateH2("hTf_other","",1000,-6,6, nmodels,0,nmodels);
          particleInfoMap.emplace( electronInfo.pdg, electronInfo );
      }
      if(fTrackingAction) fTrackingAction->SetParticleInfoMap(particleInfoMap);
  }

  // if user did not provide an output file name, create one
  if(fOutputFileName.empty())
  {
      fOutputFileName = analysisManager->GetFileName();
    const std::string phys = fInputArgs->physics_list;
    const bool airECAL = fInputArgs->airECAL;

    // check position of '.'
    std::size_t dotPos = fOutputFileName.find_last_of('.');

    if (std::string::npos != dotPos ) {
        // if it has extension
        std::string basename  = fOutputFileName.substr(0, dotPos);
        std::string extension = fOutputFileName.substr(dotPos); // including '.'

        fOutputFileName = basename + "_" + phys + "_airECAL" + std::to_string(airECAL) + extension;
    } else {
        // if no extension
        fOutputFileName = fOutputFileName + "_" + phys + "_airECAL" + std::to_string(airECAL);
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
