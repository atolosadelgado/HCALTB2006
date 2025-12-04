#include "YourSteppingAction.hh"
#include "YourEventAction.hh"


YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) {

    }


YourSteppingAction::~YourSteppingAction() {

}


void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

}
