#include "YourSteppingAction.hh"

#include "YourEventAction.hh"
#include "YourVCaloResponse.hh"

#include "G4Step.hh"

YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) { }


YourSteppingAction::~YourSteppingAction() {}

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {
    this->UpdateLeakedEnergy(theStep);

    const G4double eDep = theStep->GetTotalEnergyDeposit();
    if(eDep<=0.0) return;
    G4LogicalVolume * lv = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();

    double radius = YourVCaloResponse::Calculate_hitpos_to_shower_axis_distance(theStep, fYourEventAction->GetPrimaryVertexDir(), fYourEventAction->GetPrimaryVertexPos());
    fYourEventAction->UpdateProfileHistograms(lv, eDep, radius);
    fYourEventAction->UpdateTotalEnergyPerRegion(lv->GetRegion(), eDep);
}

void YourSteppingAction::UpdateLeakedEnergy(const G4Step * theStep)
{
    // if step is not crossing a boundary, return right away
    if (theStep->GetPostStepPoint()->GetStepStatus() != fGeomBoundary) return;

    // check if the boundary crossing is Calo-> out (fTBHCalPV)
    if( fCaloPV   == theStep->GetPreStepPoint()->GetPhysicalVolume() &&
        fTBHCalPV == theStep->GetPostStepPoint()->GetPhysicalVolume())
        fYourEventAction->UpdateCaloEfluxOut(theStep->GetTrack()->GetKineticEnergy());
    // check if the boundary crossing is out(fTBHCalPV -> Calo
    // exclude primary particle
    if( 0 != theStep->GetTrack()->GetParentID() &&
        fTBHCalPV == theStep->GetPreStepPoint()->GetPhysicalVolume() &&
        fCaloPV   == theStep->GetPostStepPoint()->GetPhysicalVolume())
        fYourEventAction->UpdateCaloEfluxIn(theStep->GetTrack()->GetKineticEnergy());
}
