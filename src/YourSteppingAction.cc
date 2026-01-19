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
    G4Material * mat = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial();
    if( fEcalRegion == current_region && fEcalMat == mat)
    {

        double t = theStep->GetPostStepPoint()->GetGlobalTime();
        if ( fYourEventAction->IsTimeWithinEventTimeWindow(t) ){
            fYourEventAction->AddVisibleEnergyECAL(theStep->GetTotalEnergyDeposit());
        }
        else{
            theStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }

    }
    if( fHcalRegion == current_region && fHcalMat == mat)
    {
        double t = theStep->GetPostStepPoint()->GetGlobalTime();
        if ( fYourEventAction->IsTimeWithinEventTimeWindow(t) )
        {
            G4double evis = fHCAL_emsaturation->GetEnergy(theStep);
            fYourEventAction->AddVisibleEnergyHCAL(evis);
        }
        else{
            theStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
    }
}

#include "G4GeometryManager.hh"
void YourSteppingAction::FindRegionsAndMaterials()
{
    auto geomgr = G4GeometryManager::GetInstance();
    if(not geomgr || not geomgr->IsGeometryClosed()) return;

        fEcalRegion = G4RegionStore::GetInstance()->GetRegion("EcalRegion");
    if (!fEcalRegion) {
        G4Exception("YourSteppingAction",
                    "ECAL001",
                    JustWarning,
                    "ECAL region not found");
    }

    fHcalRegion = G4RegionStore::GetInstance()->GetRegion("HcalRegion");
    if (!fHcalRegion) {
        G4Exception("YourSteppingAction",
                    "HCAL001",
                    JustWarning,
                    "HCAL region not found");
    }

    fEcalMat = G4Material::GetMaterial("E_PbWO4");
    if (!fEcalMat) {
        G4Exception("YourSteppingAction",
                    "MAT001",
                    JustWarning,
                    "ECAL material not found");
    }

    fHcalMat = G4Material::GetMaterial("Scintillator");
    if (!fHcalMat) {
        G4Exception("YourSteppingAction",
                    "MAT002",
                    JustWarning,
                    "HCAL material not found");
    }
}

