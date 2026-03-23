#include "YourSteppingAction.hh"

#include "YourEventAction.hh"
#include "YourVCaloResponse.hh"

#include "G4Step.hh"

YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) { }


YourSteppingAction::~YourSteppingAction() {}

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    const G4double eDep = theStep->GetTotalEnergyDeposit();
    if(eDep<=0.0) return;
    G4LogicalVolume * lv = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();

    double radius = YourVCaloResponse::Calculate_hitpos_to_shower_axis_distance(theStep, fYourEventAction->GetPrimaryVertexDir(), fYourEventAction->GetPrimaryVertexPos());
    fYourEventAction->UpdateProfileHistograms(lv, eDep, radius);
}


