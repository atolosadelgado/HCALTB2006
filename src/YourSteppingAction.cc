#include "YourSteppingAction.hh"
#include "YourEventAction.hh"
#include "G4Step.hh"
#include "G4RegionStore.hh"


YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) {


    }


YourSteppingAction::~YourSteppingAction() {

}


void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    G4Region * current_region = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
    if( ecal_region == current_region )
        fYourEventAction->AddVisibleEnergyECAL(theStep->GetTotalEnergyDeposit());
    if( hcal_region == current_region )
        fYourEventAction->AddVisibleEnergyHCAL(theStep->GetTotalEnergyDeposit());
}
