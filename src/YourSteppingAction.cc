#include "YourSteppingAction.hh"

#include "YourEventAction.hh"

#include "G4Step.hh"

YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) { }


YourSteppingAction::~YourSteppingAction() {}

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    const G4double eDep = theStep->GetTotalEnergyDeposit();
    if(eDep<=0.0) return;
    G4LogicalVolume * lv = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
    fYourEventAction->AddEnergy(lv, eDep);
}
