#include "YourEventAction.hh"
#include "SimpleCaloSD.h"

#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

void YourEventAction::EndOfEventAction(const G4Event*) {
    auto* sdManager = G4SDManager::GetSDMpointer();

    auto* ecalSD =
        static_cast<SimpleCaloSD*>(sdManager->FindSensitiveDetector("ecalSD"));

    auto* hcalSD =
        static_cast<SimpleCaloSD*>(sdManager->FindSensitiveDetector("hcalSD"));

    auto* ana = G4AnalysisManager::Instance();
    ana->FillNtupleDColumn(0, ecalSD->Get_event_energy());
    ana->FillNtupleDColumn(1, hcalSD->Get_event_energy());
    ana->AddNtupleRow();
}

