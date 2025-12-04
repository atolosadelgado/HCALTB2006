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

}


void YourEventAction::EndOfEventAction(const G4Event* evt) {

}


