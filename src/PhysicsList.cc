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



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(0 * CLHEP::mm, "proton");
  SetCutValue(1 * CLHEP::mm, "e-");
  SetCutValue(1 * CLHEP::mm, "e+");
  SetCutValue(1 * CLHEP::mm, "gamma");
}
