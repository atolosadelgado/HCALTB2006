#include "YourEventAction.hh"
#include "YourRunAction.hh"
#include "YourPrimaryGenerator.hh"

#include "G4EventManager.hh"


YourEventAction::YourEventAction(YourRunAction * myRunAction, YourPrimaryGenerator * gen)
  : G4UserEventAction(),
  fRunAction(myRunAction), fPrimaryGenerator(gen) {

  }


YourEventAction::~YourEventAction() {

}

void YourEventAction::BeginOfEventAction(const G4Event* /*anEvent*/) {
  visible_energy_ecal = 0;
  visible_energy_hcal = 0;

}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/) {
  fRunAction->FillEventEnergyECAL(visible_energy_ecal);
  fRunAction->FillEventEnergyHCAL(visible_energy_hcal);


}


