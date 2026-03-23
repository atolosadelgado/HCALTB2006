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
    fEventAction->SetLayerInfo(fDetector->GetLayerInfo() );

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
  analysisManager->CreateNtupleDColumn("Eprofile", fEventAction->GetEnergyProfileVector());

  analysisManager->FinishNtuple();

  // create histograms of initial and final energy, and lifetime of particles
  {
      int nmodels = G4PhysicsModelCatalog::Entries();
      YourParticleInfoMap particleInfoMap;
      std::vector<std::pair<int, G4String>> particles = {
            {11,   "electron"},   // e-
            {22,   "gamma"},
            {2112, "neutron"},
            {211,  "piPlus"},
            {-211, "piMinus"},
            {111,  "pi0"},
            {2212, "proton"},
            {YourParticleInfo::PDG_OTHER, "others"}

      };
      for (const auto& [pdg, name] : particles) {
            YourParticleInfo info;
            info.pdg = pdg;

            info.hIDe0 = analysisManager->CreateH2(
                "hE0_" + name, "", 2500, -15, 10, nmodels, 0, nmodels);

            info.hIDef = analysisManager->CreateH2(
                "hEf_" + name, "", 2500, -15, 10, nmodels, 0, nmodels);

            info.hIDtf = analysisManager->CreateH2(
                "hTf_" + name, "", 2500, -15, 10, nmodels, 0, nmodels);

            particleInfoMap.emplace(pdg, info);
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
