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
    // double ecal_eventEnergyResponse = ecal_energy_MeV / fPrimaryGenerator->E0_MeV;

    double hcal_energy_MeV = hcalSD->Get_event_energy() / CLHEP::MeV;
    // double hcal_eventEnergyResponse = hcal_energy_MeV / fPrimaryGenerator->E0_MeV;

    auto* ana = G4AnalysisManager::Instance();
    ana->FillNtupleDColumn(0, ecal_energy_MeV );
    ana->FillNtupleDColumn(1, hcalSD->event_energy);
    // ana->FillNtupleDColumn(2, hcalSD->event_energy_birk);
    // ana->FillNtupleDColumn(3, hcalSD->event_energy_0wt);
    ana->FillNtupleDColumn(2, hcalSD->event_energy_raw);
    ana->FillNtupleDColumn(3, hcalSD->event_energy_raw_gamma);
    ana->FillNtupleDColumn(4, hcalSD->event_energy_raw_electron);
    ana->FillNtupleDColumn(5, hcalSD->event_energy_raw_neutron);
    ana->FillNtupleDColumn(6, hcalSD->event_energy_raw_proton);
    ana->FillNtupleDColumn(7, hcalSD->event_energy_raw_pion);
    ana->FillNtupleDColumn(8, hcalSD->event_nparticles);
    ana->FillNtupleDColumn(9, hcalSD->event_nparticles_gamma);
    ana->FillNtupleDColumn(10, hcalSD->event_nparticles_electron);
    ana->FillNtupleDColumn(11, hcalSD->event_nparticles_neutron);
    ana->FillNtupleDColumn(12, hcalSD->event_nparticles_proton);
    ana->FillNtupleDColumn(13, hcalSD->event_nparticles_pion);
    ana->FillNtupleDColumn(14, GetEventTime() );


    ana->AddNtupleRow();
}

void YourEventAction::UpdateTime(double t)
{
    this->time_first_hit = std::min(this->time_first_hit, t);
    this->time_last_hit  = std::max(this->time_last_hit, t);
}
