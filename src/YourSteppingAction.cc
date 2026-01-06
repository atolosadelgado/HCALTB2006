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


std::vector<std::string> sensitive_material_v = {"Scintillator", "E_PbWO4" };

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    G4Region * current_region = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
    auto matname = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
    if( ecal_region == current_region && "E_PbWO4" == matname)
        fYourEventAction->AddVisibleEnergyECAL(theStep->GetTotalEnergyDeposit());
    if( hcal_region == current_region && "Scintillator" == matname)
        fYourEventAction->AddVisibleEnergyHCAL(theStep->GetTotalEnergyDeposit());
}
