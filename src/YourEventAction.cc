#include "YourEventAction.hh"
#include "YourPrimaryGenerator.hh"
#include "ECalSD.h"
#include "HCalSD.h"

#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "Randomize.hh"

void YourEventAction::BeginOfEventAction(const G4Event*)
{
      // std::ofstream ofile("myevent_info.txt");
  // ofile << "Random engine: "
              // << CLHEP::HepRandom::getTheEngine()->name()
              // << std::endl;
  // long seed = 123456789123456789;

  // G4Random::setTheSeed(seed);
  // ofile << "G4Random seed : " << seed << std::endl;
  // ofile << "particle | Ekin | stepLength | Edep | volume | process | Nsecondaries" << std::endl;
}

void YourEventAction::EndOfEventAction(const G4Event*) {
        auto* sdManager = G4SDManager::GetSDMpointer();

    auto* ecalSD =
        static_cast<ECalSD*>(sdManager->FindSensitiveDetector("ecalSD"));

    auto* hcalSD =
        static_cast<HCalSD*>(sdManager->FindSensitiveDetector("hcalSD"));

//     double ecal_energy_MeV = ecalSD->Get_event_energy() / CLHEP::MeV;
    // double ecal_eventEnergyResponse = ecal_energy_MeV / fPrimaryGenerator->E0_MeV;

//     double hcal_energy_MeV = hcalSD->Get_event_energy() / CLHEP::MeV;
    // double hcal_eventEnergyResponse = hcal_energy_MeV / fPrimaryGenerator->E0_MeV;

    auto* ana = G4AnalysisManager::Instance();
    ana->FillNtupleDColumn(0, ecalSD->Get_event_energy(false) );
    ana->FillNtupleDColumn(1, hcalSD->Get_event_energy(false) );
    ana->FillNtupleDColumn(2, ecalSD->Get_event_energy() );
    ana->FillNtupleDColumn(3, hcalSD->Get_event_energy() );


//     ana->FillNtupleDColumn(1, hcalSD->event_energy);
//     ana->FillNtupleDColumn(2, hcalSD->event_energy_raw);
//     ana->FillNtupleDColumn(3, hcalSD->event_energy_raw_gamma);
//     ana->FillNtupleDColumn(4, hcalSD->event_energy_raw_electron);
//     ana->FillNtupleDColumn(5, hcalSD->event_energy_raw_neutron);
//     ana->FillNtupleDColumn(6, hcalSD->event_energy_raw_proton);
//     ana->FillNtupleDColumn(7, hcalSD->event_energy_raw_pion);
//     ana->FillNtupleDColumn(8, hcalSD->event_nparticles);
//     ana->FillNtupleDColumn(9, hcalSD->event_nparticles_gamma);
//     ana->FillNtupleDColumn(10, hcalSD->event_nparticles_electron);
//     ana->FillNtupleDColumn(11, hcalSD->event_nparticles_neutron);
//     ana->FillNtupleDColumn(12, hcalSD->event_nparticles_proton);
//     ana->FillNtupleDColumn(13, hcalSD->event_nparticles_pion);
    // ana->FillNtupleDColumn(14, GetEventTime() );

//     ana->FillNtupleDColumn(17, ecalSD->event_energy_raw);
//     ana->FillNtupleDColumn(18, ecalSD->event_energy_raw_gamma);
//     ana->FillNtupleDColumn(19, ecalSD->event_energy_raw_electron);
//     ana->FillNtupleDColumn(20, ecalSD->event_energy_raw_neutron);
//     ana->FillNtupleDColumn(21, ecalSD->event_energy_raw_proton);
//     ana->FillNtupleDColumn(22, ecalSD->event_energy_raw_pion);
//     ana->FillNtupleDColumn(23, ecalSD->event_nparticles);
//     ana->FillNtupleDColumn(24, ecalSD->event_nparticles_gamma);
//     ana->FillNtupleDColumn(25, ecalSD->event_nparticles_electron);
//     ana->FillNtupleDColumn(26, ecalSD->event_nparticles_neutron);
//     ana->FillNtupleDColumn(27, ecalSD->event_nparticles_proton);
//     ana->FillNtupleDColumn(28, ecalSD->event_nparticles_pion);
//     ana->FillNtupleDColumn(29, ecalSD->fElectronCount);
//     ana->FillNtupleDColumn(30, hcalSD->fElectronCount);
//     ana->FillNtupleDColumn(31, ecalSD->GetPrimaryEnergy());
    // ana->FillNtupleDColumn(32-34, primary pxpypz);

    ana->AddNtupleRow();
    // ResetEventTime();
    // this->fGammaE0_ecal.clear();
    // this->fGammaModelIndex_ecal.clear();
    // this->fNeutronEfinal.clear();
    // this->fNeutronTfinal.clear();
}

// void YourEventAction::UpdateTime(double t)
// {
//     this->time_first_hit = std::min(this->time_first_hit, t);
//     this->time_last_hit  = std::max(this->time_last_hit, t);
// }
