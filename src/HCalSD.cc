#include "HCalSD.h"
#include "SDutils.hh"

#include <iostream>

#include "YourRunAction.hh"
#include "YourActionInitialization.hh"

#include "G4EventManager.hh"
#include "YourEventAction.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Neutron.hh"
#include "G4PionMinus.hh"
#include "G4PionPlus.hh"
#include "G4Proton.hh"

G4bool HCalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    double edep = aStep->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

//     auto * user_evt_action = G4EventManager::GetEventManager()->GetUserEventAction();
//     // debug...
//     if(user_evt_action)
//     {
//         YourEventAction * evt = dynamic_cast<YourEventAction*>(user_evt_action);
//         G4Track * thisTrack = aStep->GetTrack();
//         evt->UpdateTime(thisTrack->GetGlobalTime());
//         event_energy_raw += edep;
//         event_nparticles++;
//         const G4ParticleDefinition * thisPartDefinition = thisTrack->GetParticleDefinition();
//         if(G4Gamma::Gamma() == thisPartDefinition )
//         {
//             event_nparticles_gamma++;
//             event_energy_raw_gamma+= edep;
//         }
//         else if(G4Electron::Electron() == thisPartDefinition )
//         {
//             event_nparticles_electron++;
//             event_energy_raw_electron+= edep;
//         }
//         else if(G4Neutron::Neutron() == thisPartDefinition )
//         {
//             event_nparticles_neutron++;
//             event_energy_raw_neutron+= edep;
//         }
//         else if(G4PionMinus::PionMinus() == thisPartDefinition ||  G4PionPlus::PionPlus() == thisPartDefinition)
//         {
//             event_nparticles_pion++;
//             event_energy_raw_pion+= edep;
//         }
//         else if(G4Proton::Proton() == thisPartDefinition )
//         {
//             event_nparticles_proton++;
//             event_energy_raw_proton+= edep;
//         }
//         fPDG.push_back(thisPartDefinition->GetPDGEncoding());
//         fModelIndex.push_back(thisTrack->GetCreatorModelIndex());
//         const std::vector<const G4Track*>* secondaries =
//             aStep->GetSecondaryInCurrentStep();
//
//         for(auto sec : *secondaries)
//         {
//             if(sec->GetDefinition() == G4Electron::Electron())
//             {
//                 fElectronCount++;
//                 fElectronE0.push_back(sec->GetKineticEnergy());
//             }
//         }
//     }

    auto & prepos = aStep->GetPreStepPoint()->GetPosition();
    auto & postpos = aStep->GetPostStepPoint()->GetPosition();
    auto  avepos = 0.5*(prepos + postpos);
    double distance_hit_shower_axis = SDutils::Calculate_hitpos_to_shower_axis_distance(avepos,fPrimaryGenerator->direction0, fPrimaryGenerator->position0);
    // if(25*CLHEP::cm < distance_hit_shower_axis)
        // return false;


        // # Values of Birks constants from NIM 80 (1970) 239-244:
        // # as implemented in Geant3 required correction due to
        // # biased computation of enery deposition
        // BirkC3                    = cms.double(1.75),
        // BirkC2                    = cms.double(0.142),
        // BirkC1                    = cms.double(0.0060),
    // double bunit = (CLHEP::g / (CLHEP::MeV * CLHEP::cm2));
    // 3.74491e+17 = 0.0060 * bunit
    double birk_correction = SDutils::getAttenuation(aStep, 3.74491e+17, 0.142, 1.75);
    G4LogicalVolume * lv = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
    std::string lvname = lv->GetName();
    double Wt0_correction = ("HBScintillatorLayer0In1" == lvname) || ("HBScintillatorLayer0In2" == lvname) ? 0.41 : 1.0;
    double energy_corrected = edep * birk_correction * Wt0_correction;
    event_energy += energy_corrected;
    energy_accumulator.AddHitInfo(lv, {energy_corrected, distance_hit_shower_axis});
    if(1<verbosity)
        std::cout << "\t[" + this->GetName() + "] " << edep/CLHEP::MeV << " MeV" << std::endl;

    return true; // Step properly processed
}

double HCalSD::getEnergyDeposit(G4Step* aStep) {
    return aStep->GetTotalEnergyDeposit();
}

void HCalSD::Initialize(G4HCofThisEvent*) {
    event_energy = 0.0;
    energy_accumulator.Initialize(sensitive_lv);
}

void HCalSD::EndOfEvent(G4HCofThisEvent*) {
    if(0<verbosity)
        std::cout << "Total energy [" + this->GetName() + "] " << event_energy/CLHEP::MeV << " MeV" << std::endl;
}
