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

void YourEventAction::BeginOfEventAction(const G4Event* anEvent) {
  visible_energy_ecal = 0;
  visible_energy_hcal = 0;

  if( anEvent->GetEventID() % 10 == 0 )
    std::cout << "Event\t" << anEvent->GetEventID() << std::endl;

}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/) {
  fRunAction->FillEventEnergy(visible_energy_ecal,visible_energy_hcal);
}


