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

    if(0<verbosity && G4Threading::IsMasterThread())
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
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw_gamma");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw_electron");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw_neutron");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw_proton");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse_raw_pion");
  analysisManager->CreateNtupleDColumn("HCAL_nsteps_raw");
  analysisManager->CreateNtupleDColumn("HCAL_nsteps_gamma");
  analysisManager->CreateNtupleDColumn("HCAL_nsteps_electron");
  analysisManager->CreateNtupleDColumn("HCAL_nsteps_neutron");
  analysisManager->CreateNtupleDColumn("HCAL_nsteps_proton");
  analysisManager->CreateNtupleDColumn("HCAL_nsteps_pion");
  analysisManager->CreateNtupleDColumn("time");

    auto* hcalSD =
        static_cast<HCalSD*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("hcalSD"));
  analysisManager->CreateNtupleIColumn("pdg", hcalSD->fPDG );
  analysisManager->CreateNtupleIColumn("model", hcalSD->fModelIndex );


  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw_gamma");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw_electron");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw_neutron");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw_proton");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse_raw_pion");
  analysisManager->CreateNtupleDColumn("ECAL_nsteps_raw");
  analysisManager->CreateNtupleDColumn("ECAL_nsteps_gamma");
  analysisManager->CreateNtupleDColumn("ECAL_nsteps_electron");
  analysisManager->CreateNtupleDColumn("ECAL_nsteps_neutron");
  analysisManager->CreateNtupleDColumn("ECAL_nsteps_proton");
  analysisManager->CreateNtupleDColumn("ECAL_nsteps_pion");
  analysisManager->CreateNtupleDColumn("ECAL_nelectrons");
  analysisManager->CreateNtupleDColumn("HCAL_nelectrons");
  analysisManager->CreateNtupleDColumn("primary_energy");

  analysisManager->CreateNtupleDColumn("primary_px");
  analysisManager->CreateNtupleDColumn("primary_py");
  analysisManager->CreateNtupleDColumn("primary_pz");
    auto* ecalSD =
        static_cast<ECalSD*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("ecalSD"));
  analysisManager->CreateNtupleDColumn("ECAL_electronE0", ecalSD->fElectronE0);
  analysisManager->CreateNtupleDColumn("HCAL_electronE0", hcalSD->fElectronE0);
  analysisManager->CreateNtupleIColumn("ECAL_electronModel", ecalSD->fModelIndex_electron);

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
#include "G4HadronicParameters.hh"
#include "G4EmParameters.hh"
#include "G4HadronicProcessStore.hh"
#include <fstream>
#include "G4PhysicsModelCatalog.hh"
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

    G4cout << "===== G4HadronicParameters =====\n";
    G4HadronicParameters* hadronic_params = G4HadronicParameters::Instance();
    // Energy limits
    G4cout << "MaxEnergy: " << hadronic_params->GetMaxEnergy() << "\n";

    G4cout << "MinEnergyTransitionFTF_Cascade: "
              << hadronic_params->GetMinEnergyTransitionFTF_Cascade() << "\n";
    G4cout << "MaxEnergyTransitionFTF_Cascade: "
              << hadronic_params->GetMaxEnergyTransitionFTF_Cascade() << "\n";

    G4cout << "MinEnergyTransitionQGS_FTF: "
              << hadronic_params->GetMinEnergyTransitionQGS_FTF() << "\n";
    G4cout << "MaxEnergyTransitionQGS_FTF: "
              << hadronic_params->GetMaxEnergyTransitionQGS_FTF() << "\n";

    G4cout << "MinEnergyINCLXX_Pbar: "
              << hadronic_params->GetMinEnergyINCLXX_Pbar() << "\n";
    G4cout << "MaxEnergyINCLXX_Pbar: "
              << hadronic_params->GetMaxEnergyINCLXX_Pbar() << "\n";

    G4cout << "EnergyThresholdForHeavyHadrons: "
              << hadronic_params->EnergyThresholdForHeavyHadrons() << "\n";

    // Cross section factors
    G4cout << "XSFactorNucleonInelastic: "
              << hadronic_params->XSFactorNucleonInelastic() << "\n";
    G4cout << "XSFactorNucleonElastic: "
              << hadronic_params->XSFactorNucleonElastic() << "\n";

    G4cout << "XSFactorPionInelastic: "
              << hadronic_params->XSFactorPionInelastic() << "\n";
    G4cout << "XSFactorPionElastic: "
              << hadronic_params->XSFactorPionElastic() << "\n";

    G4cout << "XSFactorHadronInelastic: "
              << hadronic_params->XSFactorHadronInelastic() << "\n";
    G4cout << "XSFactorHadronElastic: "
              << hadronic_params->XSFactorHadronElastic() << "\n";

    G4cout << "XSFactorEM: "
              << hadronic_params->XSFactorEM() << "\n";

    // Flags
    G4cout << "EnableBCParticles: "
              << hadronic_params->EnableBCParticles() << "\n";
    G4cout << "EnableHyperNuclei: "
              << hadronic_params->EnableHyperNuclei() << "\n";
    G4cout << "ApplyFactorXS: "
              << hadronic_params->ApplyFactorXS() << "\n";

    G4cout << "EnableCRCoalescence: "
              << hadronic_params->EnableCRCoalescence() << "\n";

    G4cout << "EnableIntegralInelasticXS: "
              << hadronic_params->EnableIntegralInelasticXS() << "\n";
    G4cout << "EnableIntegralElasticXS: "
              << hadronic_params->EnableIntegralElasticXS() << "\n";

    G4cout << "EnableDiffDissociationForBGreater10: "
              << hadronic_params->EnableDiffDissociationForBGreater10() << "\n";

    G4cout << "EnableCoherentChargeExchange: "
              << hadronic_params->EnableCoherentChargeExchange() << "\n";

    G4cout << "EnableNeutronGeneralProcess: "
              << hadronic_params->EnableNeutronGeneralProcess() << "\n";

    // Verbosity
    G4cout << "VerboseLevel: "
              << hadronic_params->GetVerboseLevel() << "\n";

    // Energy-momentum conservation parameters
    G4cout << "EPRelativeLevel: "
              << hadronic_params->GetEPRelativeLevel() << "\n";
    G4cout << "EPAbsoluteLevel: "
              << hadronic_params->GetEPAbsoluteLevel() << "\n";
    G4cout << "EPReportLevel: "
              << hadronic_params->GetEPReportLevel() << "\n";

    G4cout << "BinaryDebug: "
              << hadronic_params->GetBinaryDebug() << "\n";

    // Environment variables
    G4cout << "DirPARTICLEXS: "
              << hadronic_params->GetDirPARTICLEXS() << "\n";
    G4cout << "PhysListDocDir: "
              << hadronic_params->GetPhysListDocDir() << "\n";
    G4cout << "PhysListName: "
              << hadronic_params->GetPhysListName() << "\n";

    // Thresholds
    G4cout << "NeutronKineticEnergyThresholdForSVT: "
              << hadronic_params->GetNeutronKineticEnergyThresholdForSVT() << "\n";

    G4cout << "TimeThresholdForRadioactiveDecay: "
              << hadronic_params->GetTimeThresholdForRadioactiveDecay() << "\n";

    G4cout << "================================\n";

    G4cout << "===== G4EmParameters =====\n";
    G4EmParameters::Instance()->StreamInfo(G4cout);
    G4EmParameters::Instance()->Dump();
    G4cout << "================================\n";

    G4cout << "===== G4HadronicProcessStore =====\n";
    G4HadronicProcessStore::Instance()->Dump(1);

    G4cout << "================================\n";
    G4PhysicsModelCatalog::PrintAllInformation();
    dumpFile.flush();
    std::streambuf* coutbuf = G4cout.rdbuf();
    G4cout.rdbuf(coutbuf);
}
