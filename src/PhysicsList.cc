#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"


#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "CMSHadronPhysicsFTFP_BERT.hh"

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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(0 * CLHEP::mm, "proton");
  SetCutValue(1 * CLHEP::mm, "e-");
  SetCutValue(1 * CLHEP::mm, "e+");
  SetCutValue(1 * CLHEP::mm, "gamma");
}
