#include "YourSteppingAction.hh"
#include "YourEventAction.hh"
#include "G4Step.hh"


YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) {

    }


YourSteppingAction::~YourSteppingAction() {

}


void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    fYourEventAction->AddVisibleEnergy(theStep->GetTotalEnergyDeposit());

}
