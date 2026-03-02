#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
// #include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"


#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "CMSHadronPhysicsFTFP_BERT.hh"

#include "G4HadronicParameters.hh"
#include "G4EmParameters.hh"

PhysicsList::PhysicsList()
{
  G4int ver = 1;
  SetVerboseLevel(ver);

  bool emPhys = true;
  bool hadPhys = true;

  // transition energy Bertini/FTFP from 3 to 6:12 GeVP

  G4double minFTFP = 3*CLHEP::GeV;
  G4double maxBERT = 6*CLHEP::GeV;
  G4double maxBERTpi = 12*CLHEP::GeV;

  if (emPhys) {
    // EM Physics
    RegisterPhysics(new G4EmStandardPhysics(ver));
    // RegisterPhysics(new G4EmStandardPhysics_option4(ver));

    // Synchroton Radiation & GN Physics
    G4EmExtraPhysics* gn = new G4EmExtraPhysics(ver);
    RegisterPhysics(gn);
  }

  // Decays
  RegisterPhysics(new G4DecayPhysics(ver));

  if (hadPhys) {
    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysics(ver));

    // Hadron Physics
    RegisterPhysics(new CMSHadronPhysicsFTFP_BERT(minFTFP, maxBERT, maxBERTpi, minFTFP, maxBERT));

    // Stopping Physics
    RegisterPhysics(new G4StoppingPhysics(ver));

    // Ion Physics
    RegisterPhysics(new G4IonPhysics(ver));
  }

  // // ===== G4HadronicParameters =====
  using CLHEP::GeV;
  G4HadronicParameters * hadronic_parameters = G4HadronicParameters::Instance();

  //   MaxEnergy                         = 100000 GeV
  // FTF-Cascade transition:
  //   MinEnergy = 3 GeV
  //   MaxEnergy = 6 GeV
  // QGS-FTF transition:
  //   MinEnergy = 12 GeV
  //   MaxEnergy = 25 GeV
  // INCLXX (pbar):
  //   MinEnergy = 0 GeV
  //   MaxEnergy = 10 GeV
  // EnergyThresholdForHeavyHadrons     = 1.1 GeV
  hadronic_parameters->SetMaxEnergy(100000*GeV);
  hadronic_parameters->SetMinEnergyTransitionFTF_Cascade(3*GeV);
  hadronic_parameters->SetMaxEnergyTransitionFTF_Cascade(6*GeV);
  hadronic_parameters->SetMinEnergyTransitionQGS_FTF(12*GeV);
  hadronic_parameters->SetMaxEnergyTransitionQGS_FTF(25*GeV);
  hadronic_parameters->SetMinEnergyINCLXX_Pbar(0);
  hadronic_parameters->SetMaxEnergyINCLXX_Pbar(10*GeV);
  hadronic_parameters->SetEnergyThresholdForHeavyHadrons(1.1*GeV);

  // XS Nucleon Inelastic               = 1
  // XS Nucleon Elastic                 = 1
  // XS Pion Inelastic                  = 1
  // XS Pion Elastic                    = 1
  // XS Hadron Inelastic                = 1
  // XS Hadron Elastic                  = 1
  // XS EM                              = 1
  hadronic_parameters->SetXSFactorNucleonInelastic(1.);
  hadronic_parameters->SetXSFactorNucleonElastic(1.);
  hadronic_parameters->SetXSFactorPionInelastic(1.);
  hadronic_parameters->SetXSFactorPionElastic(1.);
  hadronic_parameters->SetXSFactorHadronInelastic(1.);
  hadronic_parameters->SetXSFactorHadronElastic(1.);
  hadronic_parameters->SetXSFactorEM(1.);
  // Enable BC particles                = 0
  // Enable HyperNuclei                 = 0
  // Apply XS factor                    = 0
  // Enable CR coalescence              = 0
  // Integral Inelastic XS              = 1
  // Integral Elastic XS                = 1
  // Diff. Dissociation (B>10)          = 0
  // Neutron General Process            = 0
  // Coherent Charge Exchange           = 0
  hadronic_parameters->SetEnableBCParticles(false);
  hadronic_parameters->SetEnableHyperNuclei(false);
  hadronic_parameters->SetApplyFactorXS(false);
  hadronic_parameters->SetEnableCRCoalescence(false);
  hadronic_parameters->SetEnableIntegralElasticXS(true);
  hadronic_parameters->SetEnableIntegralInelasticXS(true);
  hadronic_parameters->SetEnableDiffDissociationForBGreater10(false);
  hadronic_parameters->SetEnableNeutronGeneralProcess(false);
  hadronic_parameters->SetEnableCoherentChargeExchange(false);

  // Neutron SVT threshold              = -1 MeV
  // Radioactive decay time threshold   = -1e-09 s
  hadronic_parameters->SetNeutronKineticEnergyThresholdForSVT(-1*CLHEP::MeV);
  hadronic_parameters->SetTimeThresholdForRadioactiveDecay(-1e-6*CLHEP::s);

  G4EmParameters * em_parameters = G4EmParameters::Instance();
  // LPM effect enabled                                 1
  // Enable creation and use of sampling tables         0
  // Apply cuts on all EM processes                     0
  // Use combined TransportationWithMsc                 Disabled
  // Use general process                                0
  // Enable linear polarisation for gamma               0
  // Enable photoeffect sampling below K-shell          1
  // Enable sampling of quantum entanglement            0
  em_parameters->SetLPM(true);
  em_parameters->SetEnableSamplingTable(false);
  em_parameters->SetApplyCuts(false);
  em_parameters->SetTransportationWithMsc(G4TransportationWithMscType::fDisabled);
  em_parameters->SetGeneralProcessActive(false);
  em_parameters->SetEnablePolarisation(false);
  em_parameters->SetPhotoeffectBelowKShell(true);
  em_parameters->SetQuantumEntanglement(false);
  // X-section factor for integral approach             0.8 ->which method??
  // Min kinetic energy for tables                      100 eV
  // Max kinetic energy for tables                      100 TeV
  // Number of bins per decade of a table               7
  // Verbose level                                      1
  // Verbose level for worker thread                    0
  // Bremsstrahlung energy threshold above which
  //   primary e+- is added to the list of secondary    100 TeV
  // Bremsstrahlung energy threshold above which primary
  //   muon/hadron is added to the list of secondary    100 TeV
  em_parameters->SetMinEnergy(100*CLHEP::eV);
  em_parameters->SetMaxEnergy(100*CLHEP::TeV);
  em_parameters->SetNumberOfBinsPerDecade(7);
  em_parameters->SetBremsstrahlungTh(100*CLHEP::TeV);
  em_parameters->SetMuHadBremsstrahlungTh(100*CLHEP::TeV);
  // Lowest triplet kinetic energy                      1 MeV
  // Enable sampling of gamma linear polarisation       0
  // 5D gamma conversion model type                     0
  // 5D gamma conversion model on isolated ion          0
  // Livermore data directory                           epics_2017
  em_parameters->SetLowestTripletEnergy(1*CLHEP::MeV);
  em_parameters->SetEnablePolarisation(false);
  em_parameters->SetConversionType(0);
  em_parameters->SetOnIsolated(false);
  em_parameters->SetLivermoreDataDir("epics_2017");

    // ============================================================
    //                Ionisation Parameters
    // ============================================================

    // Step functions
    using CLHEP::mm;
    using CLHEP::keV;
    em_parameters->SetStepFunction(0.2, 1.0*mm);              // e+/e-
    em_parameters->SetStepFunctionMuHad(0.2, 0.1*mm);         // muons/hadrons
    em_parameters->SetStepFunctionLightIons(0.2, 0.1*mm);     // light ions
    em_parameters->SetStepFunctionIons(0.2, 0.1*mm);          // general ions

    // Lowest kinetic energies
    em_parameters->SetLowestElectronEnergy(1.0*keV);
    em_parameters->SetLowestMuHadEnergy(1.0*keV);

    // ICRU90
    em_parameters->SetUseICRU90Data(false);

    // dE/dx fluctuations
    em_parameters->SetLossFluctuations(true);
    em_parameters->SetFluctuationType(fUniversalFluctuation);

    // Birks saturation
    em_parameters->SetBirksActive(false);

    // CSDA range
    em_parameters->SetBuildCSDARange(false);
    em_parameters->SetUseCutAsFinalRange(false);
    em_parameters->SetMaxEnergyForCSDARange(1.0*GeV);

    // NIEL
    em_parameters->SetMaxNIELEnergy(0.0);

    // Linear loss limit
    em_parameters->SetLinearLossLimit(0.01);

    // Mu pair production from file, false -> which method?


    // Angular generator interface
    em_parameters->ActivateAngularGeneratorForIonisation(false);

    // ============================================================
    //            Multiple Scattering Parameters
    // ============================================================

    // Step limit algorithms
    em_parameters->SetMscStepLimitType(fUseSafetyPlus);       // e+-
    em_parameters->SetMscMuHadStepLimitType(fMinimal);        // muons/hadrons

    // Lateral displacement
    em_parameters->SetLateralDisplacement(true);              // e+-
    em_parameters->SetMuHadLateralDisplacement(false);       // mu/had

    // Urban MSC options
    // em_parameters->SetUrbanUseDistanceToBoundary(true);       // alg96 equivalent --> what method?

    // Range factors
    em_parameters->SetMscRangeFactor(0.04);                   // e+-
    em_parameters->SetMscMuHadRangeFactor(0.2);               // mu/had

    // Geometry & safety factors (e+-)
    em_parameters->SetMscGeomFactor(2.5);
    em_parameters->SetMscSafetyFactor(0.6);
    em_parameters->SetMscSkin(1);
    em_parameters->SetMscLambdaLimit(1.0*mm);

    // Mott correction
    em_parameters->SetUseMottCorrection(false);

    // Single vs multiple scattering transition
    em_parameters->SetFactorForAngleLimit(1.0);
    // em_parameters->SetMscThetaLimit(3.1416*CLHEP::rad); // this triggers an exception
    em_parameters->SetMscThetaLimit(CLHEP::pi*CLHEP::rad);


    // Upper energy limit for e+- MSC
    em_parameters->SetMscEnergyLimit(100.0*CLHEP::MeV);

    // Electron single scattering model
    em_parameters->SetSingleScatteringType(fWVI); // fWVI = 0

    // Nuclear form-factor
    em_parameters->SetNuclearFormfactorType(fExponentialNF); // fExponentialNF = 1

    // Screening factor
    em_parameters->SetScreeningFactor(1.0);





}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(0 * CLHEP::mm, "proton");
  SetCutValue(1 * CLHEP::mm, "e-");
  SetCutValue(1 * CLHEP::mm, "e+");
  SetCutValue(1 * CLHEP::mm, "gamma");
}
