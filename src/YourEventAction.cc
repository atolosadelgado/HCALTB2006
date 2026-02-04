#include "YourEventAction.hh"
#include "YourPrimaryGenerator.hh"
#include "ECalSD.h"
#include "HCalSD.h"

#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

void YourEventAction::EndOfEventAction(const G4Event*) {
        auto* sdManager = G4SDManager::GetSDMpointer();

    auto* ecalSD =
        static_cast<ECalSD*>(sdManager->FindSensitiveDetector("ecalSD"));

    auto* hcalSD =
        static_cast<HCalSD*>(sdManager->FindSensitiveDetector("hcalSD"));

    double ecal_energy_MeV = ecalSD->Get_event_energy() / CLHEP::MeV;
    double ecal_eventEnergyResponse = ecal_energy_MeV / fPrimaryGenerator->E0_MeV;

    double hcal_energy_MeV = hcalSD->Get_event_energy() / CLHEP::MeV;
    double hcal_eventEnergyResponse = hcal_energy_MeV / fPrimaryGenerator->E0_MeV;

    auto* ana = G4AnalysisManager::Instance();
    ana->FillNtupleDColumn(0, ecal_eventEnergyResponse );
    ana->FillNtupleDColumn(1, hcal_eventEnergyResponse);
    ana->AddNtupleRow();
}

