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

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include "G4Step.hh"

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    G4Region * current_region = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
    auto matname = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
    if( ecal_region == current_region && "E_PbWO4" == matname)
        fYourEventAction->AddVisibleEnergyECAL(theStep->GetTotalEnergyDeposit());
    if( hcal_region == current_region && "Scintillator" == matname)
    {
    G4double evis = G4LossTableManager::Instance()
                        ->EmSaturation()
                        ->VisibleEnergyDepositionAtAStep(theStep);

        fYourEventAction->AddVisibleEnergyHCAL(evis);
//         if( 0 < theStep->GetTotalEnergyDeposit() )
//             std::cout << "\tevis/total = " << evis / theStep->GetTotalEnergyDeposit() << std::endl;
//             std::cout << "\tNIEL/total = " << theStep->GetNonIonizingEnergyDeposit() / theStep->GetTotalEnergyDeposit() << std::endl;
//         fYourEventAction->AddVisibleEnergyHCAL(theStep->GetTotalEnergyDeposit());
    }
}
