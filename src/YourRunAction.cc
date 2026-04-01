#include "YourRunAction.hh"

#include "YourInputArgs.hh"
#include "YourPrimaryGenerator.hh"
#include "YourEventAction.hh"
#include "YourSteppingAction.hh"
#include "YourTrackingAction.hh"
#include "YourCaloSD.hh"
#include "YourDetectorConstructor.hh"


#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4PhysicsModelCatalog.hh"
#include "G4PhysicalVolumeStore.hh"

// the following headers are needed to printout the Geant4 configuration
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
#include "G4DeexPrecoParameters.hh"
#include "G4NuclearLevelData.hh"
#include "G4PhysicsModelCatalog.hh"
#include "G4HadronicProcess.hh"
#include "G4EnergyRangeManager.hh"
#include "G4HadronicInteraction.hh"
#include <fstream>

YourRunAction::YourRunAction(const YourInputArgs * args, const YourDetectorConstructor * detector):
          G4UserRunAction(),
          fOutputFileName(args->outputFileName),
          fInputArgs(args),
          fDetector(detector)
          {
            // to make UI commands available
            auto analysisManager = G4AnalysisManager::Instance();
            (void)analysisManager; // trick to avoid error: unused variable
}


YourRunAction::~YourRunAction() {}


void YourRunAction::BeginOfRunAction(const G4Run*)
{

  if(G4Threading::IsMasterThread() && 0<verbosity)
        this->PrintGeant4Configuration();

  auto layerInfo = fDetector->GetLayerInfo();

  // initialize energy profile vector to the number of layers
  // these vectors will be tied to G4Analysis later in BeginOutputTree()
  fEnergyProfile = std::vector<double>( layerInfo.GetMaxLayerNumber() , 0.0 );
  fRadiusProfile = std::vector<double>( layerInfo.GetMaxLayerNumber() , 0.0 );


  if(fEventAction){
      fEventAction->InitializeProfileHistograms(layerInfo);
      fEventAction->InitializeRegionDefinition();
      fEventAction->SetEnergyProfileVector(&fEnergyProfile);
      fEventAction->SetRadiusProfileVector(&fRadiusProfile);
  }

  if(fSteppingAction)
  {
    G4VPhysicalVolume * calolv = G4PhysicalVolumeStore::GetInstance()->GetVolume("Calo");
    G4VPhysicalVolume * TBHCalPV = G4PhysicalVolumeStore::GetInstance()->GetVolume("TBHCal");
    if(!calolv)
        throw std::runtime_error("No physical volume named <Calo> was found");
    if(!TBHCalPV)
        throw std::runtime_error("No physical volume named <TBHCal> was found");
    fSteppingAction->SetCaloPV(calolv);
    fSteppingAction->SetTBHCalPV(TBHCalPV);
   }



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

  id = analysisManager->CreateNtupleDColumn("TotalEnergyECALregion");
  if(fEventAction) fEventAction->SetEcalTotalEnergyNtupleID(id);

  id = analysisManager->CreateNtupleDColumn("TotalEnergyHCALregion");
  if(fEventAction) fEventAction->SetHcalTotalEnergyNtupleID(id);

  id = analysisManager->CreateNtupleDColumn("CaloEfluxOut");
  if(fEventAction) fEventAction->SetCaloEfluxOutNtupleID(id);

  id = analysisManager->CreateNtupleDColumn("CaloEfluxIn");
  if(fEventAction) fEventAction->SetCaloEfluxInNtupleID(id);

  analysisManager->CreateNtupleDColumn("Eprofile", fEnergyProfile);

  analysisManager->CreateNtupleDColumn("Rprofile", fRadiusProfile);

  analysisManager->FinishNtuple();

  if(fInputArgs->secondaryTrackInfo)
      this->InitializeSecondaryTrackHistogram();

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

void YourRunAction::InitializeSecondaryTrackHistogram()
{
  // create histograms of initial and final energy, and lifetime of particles
  auto analysisManager = G4AnalysisManager::Instance();
  if(particleInfoMap.empty()){
      int nmodels = G4PhysicsModelCatalog::Entries();

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
                "hE0_" + name, "", 1000, -5, 5, nmodels+1, -1, nmodels);

            analysisManager->CreateH2(
                "hE0_" + name + "_n", "", 1000, -5, 5, nmodels+1, -1, nmodels);

            analysisManager->CreateH2(
                "hE0_" + name + "_pi", "", 1000, -5, 5, nmodels+1, -1, nmodels);

            info.hIDef = analysisManager->CreateH2(
                "hEf_" + name, "", 1000, -10, 10, nmodels+1, -1, nmodels);

            info.hIDtf = analysisManager->CreateH2(
                "hTf_" + name, "", 1000, -10, 10, nmodels+1, -1, nmodels);

            particleInfoMap.emplace(pdg, info);
      }
  }
  if(fTrackingAction) fTrackingAction->SetParticleInfoMap(particleInfoMap);
}


void YourRunAction::EndOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}

void YourRunAction::PrintGeant4Configuration()
{
    static std::ofstream dumpFile("geant4_dump.txt");
    //save buffer to restore it later
    auto cout_buf = G4cout.rdbuf();
    auto cerr_buf = G4cerr.rdbuf();
    // redirect G4cout
    G4cout.rdbuf(dumpFile.rdbuf());
    G4cerr.rdbuf(dumpFile.rdbuf());

    G4cout << "==============================" << G4endl;
    G4cout << " GEANT4 CONFIGURATION DUMP " << G4endl;
    G4cout << "==============================" << G4endl;


    G4cout << "\n=== Production Cuts Table ===\n" << G4endl;

    auto pct = G4ProductionCutsTable::GetProductionCutsTable();
    pct->DumpCouples();


    G4cout << "================================\n";
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
            G4cout << "  No production cuts attached" << G4endl;
        }
        auto * ul = region->GetUserLimits();
        if (ul)
        {
            G4Track dummyTrack;
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
        else
        {
            G4cout << "  No user limits attached" << G4endl;
        }
    }
    {

        G4cout << "================================\n";
        G4cout << "\n=== User Limits per Logical Volume ===\n" << G4endl;

        auto lvStore = G4LogicalVolumeStore::GetInstance();

        G4Track dummyTrack;
        for (auto lv : *lvStore)
        {
            auto ul = lv->GetUserLimits();
            if (ul)
            {
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
    }


    std::set<std::string> processnames;
    G4cout << "================================\n";
    G4cout << "\n=== Processes per Particle ===\n" << G4endl;

    auto particleTable = G4ParticleTable::GetParticleTable();
    auto it = particleTable->GetIterator();
    it->reset();

    while ((*it)())
    {
        auto particle = it->value();
        auto pm = particle->GetProcessManager();

        if (!pm) continue;

        G4cout << "\nParticle (pdgID): "
            << particle->GetParticleName()
            << " (" << particle->GetPDGEncoding() << ")"
            << G4endl;

        G4ProcessVector * pv = pm->GetProcessList();

        for (int i = 0; i < pm->GetProcessListLength(); ++i)
        {
            auto proc = (*pv)[i];

            G4cout << "  Process: "
                << proc->GetProcessName()
                << G4endl;

            processnames.emplace(proc->GetProcessName());

            // -------------------------------------------------
            // only for hadronic processes
            // -------------------------------------------------
            auto hadProc = dynamic_cast<G4HadronicProcess*>((*pv)[i]);

            if (!hadProc) continue;

            auto& models = hadProc->GetHadronicInteractionList();

            for (auto model : models)
            {
                G4cout << "    Model: "
                    << model->GetModelName()
                    << " | Emin = " << model->GetMinEnergy()/CLHEP::GeV << " GeV"
                    << " | Emax = " << model->GetMaxEnergy()/CLHEP::GeV << " GeV"
                    << G4endl;
            }

        }
    }
    G4cout << "\n=== List of process names ===" << G4endl;
    int counter = 0;
    for(auto & procname : processnames ){
        G4cout << "\t{\"" << procname.c_str() << "\", " << std::to_string(counter) <<"}, " <<  std::endl;
        counter++;
    }
    G4cout << "\n\n================================\n";

    G4cout << "===== G4EmParameters =====\n";
    G4EmParameters::Instance()->StreamInfo(G4cout);
    G4EmParameters::Instance()->Dump();

    G4cout << "================================\n";
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
    G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();
    deex->StreamInfo(G4cout);

    G4cout << "================================\n";
    G4cout << "===== G4HadronicProcessStore =====\n";
    G4HadronicProcessStore::Instance()->Dump(1);

    G4cout << "================================\n";
    G4PhysicsModelCatalog::PrintAllInformation();


    dumpFile.flush();
    G4cout.rdbuf(cout_buf);
    G4cerr.rdbuf(cerr_buf);
}
