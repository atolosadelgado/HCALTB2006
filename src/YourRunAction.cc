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

#include "YourPrimaryGenerator.hh"
#include "ECalSD.h"
#include "HCalSD.h"

#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
void YourRunAction::BeginOfRunAction(const G4Run*)
{
    this->ConstructOutputTree();
    this->BeginOutputTree();

    this->PrintGeant4Configuration();

    auto* hcalSD =
        static_cast<HCalSD*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("hcalSD"));
    hcalSD->fPrimaryGenerator = fPrimaryGenerator;
    auto* ecalSD =
        static_cast<ECalSD*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("ecalSD"));
    ecalSD->fPrimaryGenerator = fPrimaryGenerator;
}

void YourRunAction::EndOfRunAction(const G4Run* ){
    this->EndOutputTree();
#if HAVE_ROOT
    if (G4Threading::IsMasterThread()){
        fInputArgs->SaveToROOTfile( G4AnalysisManager::Instance()->GetFileName());
    }
#endif

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
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_birk");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_0wt");
  analysisManager->FinishNtuple();
}

void YourRunAction::BeginOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    std::string ofilename = analysisManager->GetFileName();
    const std::string phys = fInputArgs->physics_list;
    const bool airECAL     = fInputArgs->airECAL;
    const std::string birk = std::to_string(fInputArgs->saturation);

    // check position of '.'
    std::size_t dotPos = ofilename.find_last_of('.');

    if (std::string::npos != dotPos ) {
        // if it has extension
        std::string basename  = ofilename.substr(0, dotPos);
        std::string extension = ofilename.substr(dotPos); // including '.'

        ofilename = basename + "_" + phys + "_birk" + birk + "_airECAL" + std::to_string(airECAL) + extension;
    } else {
        // if no extension
        ofilename = ofilename + "_" + phys + "_birk" + birk + "_airECAL" + std::to_string(airECAL) ;
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

#include "G4RunManager.hh"
#include "G4ProductionCutsTable.hh"
#include "G4RegionStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4UserLimits.hh"
#include "G4ios.hh"

#include <fstream>
void YourRunAction::PrintGeant4Configuration()
{
    static std::ofstream dumpFile("geant4_dump.txt");
    G4cout.rdbuf(dumpFile.rdbuf());

    G4cout << "==============================" << G4endl;
    G4cout << " GEANT4 CONFIGURATION DUMP " << G4endl;
    G4cout << "==============================" << G4endl;


    G4cout << "\n=== Production Cuts Table ===\n" << G4endl;

    auto pct = G4ProductionCutsTable::GetProductionCutsTable();
    pct->DumpCouples();


    G4cout << "\n=== Regions and Production Cuts ===\n" << G4endl;

    auto regionStore = G4RegionStore::GetInstance();

    for (size_t i = 0; i < regionStore->size(); ++i)
    {
        auto region = (*regionStore)[i];
        G4cout << "Region: " << region->GetName() << G4endl;

        auto cuts = region->GetProductionCuts();
        if (cuts)
        {
            G4cout << "  gamma cut: " << cuts->GetProductionCut("gamma") << G4endl;
            G4cout << "  e- cut   : " << cuts->GetProductionCut("e-") << G4endl;
            G4cout << "  e+ cut   : " << cuts->GetProductionCut("e+") << G4endl;
            G4cout << "  proton cut: " << cuts->GetProductionCut("proton") << G4endl;
        }
        else
        {
            G4cout << "  No production cuts attached!" << G4endl;
        }
    }

    G4cout << "\n=== User Limits per Logical Volume ===\n" << G4endl;
    auto particleDef =
        G4ParticleTable::GetParticleTable()->FindParticle("geantino");

    G4DynamicParticle* dyn =
        new G4DynamicParticle(particleDef, G4ThreeVector(0,0,1), 1.0*CLHEP::GeV);

    G4Track dummyTrack(dyn, 0.0, G4ThreeVector());

    auto lvStore = G4LogicalVolumeStore::GetInstance();

    for (auto lv : *lvStore)
    {
        auto ul = lv->GetUserLimits();
        if (ul)
        {
            // G4cout << "LogicalVolume: " << lv->GetName() << G4endl;
            // G4cout << "  MaxAllowedStep: " << ul->GetMaxAllowedStep() << G4endl;
            // G4cout << "  MaxTrackLength: " << ul->GetUserMaxTrackLength() << G4endl;
            // G4cout << "  MaxTime      : " << ul->GetUserMaxTime() << G4endl;
            // G4cout << "  MinEkine     : " << ul->GetUserMinEkine() << G4endl;
            // G4cout << "  MinRange     : " << ul->GetUserMinRange() << G4endl;


            G4cout << "LogicalVolume: " << lv->GetName() << G4endl;
            G4cout << "  MaxAllowedStep: "
                  << ul->GetMaxAllowedStep(dummyTrack) << G4endl;
            G4cout << "  MaxTrackLength: "
                  << ul->GetUserMaxTrackLength(dummyTrack) << G4endl;
            G4cout << "  MaxTime: "
                  << ul->GetUserMaxTime(dummyTrack) << G4endl;
            G4cout << "  MinEkine: "
                  << ul->GetUserMinEkine(dummyTrack) << G4endl;
            G4cout << "  MinRange: "
                  << ul->GetUserMinRange(dummyTrack) << G4endl;
        }
    }


    G4cout << "\n=== Processes per Particle ===\n" << G4endl;

    auto particleTable = G4ParticleTable::GetParticleTable();
    auto it = particleTable->GetIterator();
    it->reset();

    while ((*it)())
    {
        auto particle = it->value();
        auto pm = particle->GetProcessManager();

        if (!pm) continue;

        G4cout << "\nParticle: " << particle->GetParticleName() << G4endl;

        auto pv = pm->GetProcessList();
        for (int i = 0; i < pm->GetProcessListLength(); ++i)
        {
            G4cout << "  Process: "
                   << (*pv)[i]->GetProcessName()
                   << G4endl;
        }
    }
    dumpFile.flush();
    std::streambuf* coutbuf = std::cout.rdbuf();
    G4cout.rdbuf(coutbuf);
}
