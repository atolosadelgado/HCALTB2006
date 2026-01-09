#include "YourSteppingAction.hh"
#include "YourEventAction.hh"
#include "G4Step.hh"
#include "G4RegionStore.hh"

#include "YourEMSaturation.hh"

YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction)
{
    // by default, no saturation effects
    this->SetSaturationNone();
}

void YourSteppingAction::SetSaturationNone()
{
    fHCAL_emsaturation = std::make_unique<YourEMSaturationNone>();
}
void YourSteppingAction::SetSaturationG4Birk()
{
    fHCAL_emsaturation = std::make_unique<YourEMSaturationG4Birk>();
}
void YourSteppingAction::SetSaturationCMSBirk()
{
    fHCAL_emsaturation = std::make_unique<YourEMSaturationCMSBirk>();
}



YourSteppingAction::~YourSteppingAction() {

}

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include "G4Step.hh"
#include "G4Material.hh"

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    G4Region * current_region = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
    auto mat = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial();
    if( ecal_region == current_region && ecal_sensitivemat == mat)
        fYourEventAction->AddVisibleEnergyECAL(theStep->GetTotalEnergyDeposit());
    if( hcal_region == current_region && hcal_sensitivemat == mat)
    {
    G4double evis = fHCAL_emsaturation->GetEnergy(theStep);

        fYourEventAction->AddVisibleEnergyHCAL(evis);
//         if( 0 < theStep->GetTotalEnergyDeposit() )
//             std::cout << "\tevis/total = " << evis / theStep->GetTotalEnergyDeposit() << std::endl;
//             std::cout << "\tNIEL/total = " << theStep->GetNonIonizingEnergyDeposit() / theStep->GetTotalEnergyDeposit() << std::endl;
//         fYourEventAction->AddVisibleEnergyHCAL(theStep->GetTotalEnergyDeposit());
    }
}
