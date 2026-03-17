#include "ECalSD.h"
#include "YourPrimaryGenerator.hh"
#include "SDutils.hh"

#include <iostream>

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4ThreeVector.hh"
#include "G4EventManager.hh"
#include "G4EventManager.hh"
#include "YourEventAction.hh"
#include "SDutils.hh"

#include "G4EventManager.hh"
#include "YourEventAction.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Neutron.hh"
#include "G4PionMinus.hh"
#include "G4PionPlus.hh"
#include "G4Proton.hh"

// Naive implementation
G4bool ECalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    double edep = aStep->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    const G4Track * thisTrack = aStep->GetTrack();
    if (thisTrack->GetParentID() == 0) UpdatePrimaryEnergy(aStep->GetPreStepPoint()->GetKineticEnergy() /*thisTrack->GetVertexKineticEnergy()*/);

    // debug...
    {
        event_energy_raw += edep;
        event_nparticles++;
        const G4ParticleDefinition * thisPartDefinition = thisTrack->GetParticleDefinition();
        if(G4Gamma::Gamma() == thisPartDefinition )
        {
            event_nparticles_gamma++;
            event_energy_raw_gamma+= edep;
        }
        else if(G4Electron::Electron() == thisPartDefinition )
        {
            event_nparticles_electron++;
            event_energy_raw_electron+= edep;
        }
        else if(G4Neutron::Neutron() == thisPartDefinition )
        {
            event_nparticles_neutron++;
            event_energy_raw_neutron+= edep;
        }
        else if(G4PionMinus::PionMinus() == thisPartDefinition ||  G4PionPlus::PionPlus() == thisPartDefinition)
        {
            event_nparticles_pion++;
            event_energy_raw_pion+= edep;
        }
        else if(G4Proton::Proton() == thisPartDefinition )
        {
            event_nparticles_proton++;
            event_energy_raw_proton+= edep;
        }
        // fPDG.push_back(thisPartDefinition->GetPDGEncoding());
        // fModelIndex.push_back(thisTrack->GetCreatorModelIndex());

        const std::vector<const G4Track*>* secondaries =
            aStep->GetSecondaryInCurrentStep();

        for(auto sec : *secondaries)
        {
            if(sec->GetDefinition() == G4Electron::Electron())
            {
                fElectronCount++;
                fElectronE0.push_back(sec->GetKineticEnergy());
                G4int this_modelIndex_electron = sec->GetCreatorModelIndex();
                fModelIndex_electron.push_back(this_modelIndex_electron);
                // G4cout << " New ECAL electron, model : " << this_modelIndex_electron << std::endl;

                // if photo
                if(G4int(3) == this_modelIndex_electron)
                {
                    fGammaPhotoE0.push_back( aStep->GetTrack()->GetVertexKineticEnergy());
                    fModelIndex_gammaPhoto.push_back(aStep->GetTrack()->GetCreatorModelIndex());
                    // G4cout << " \t\t : " << aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() << std::endl;
                    // G4cout << " \t\t : " << aStep->GetTrack()->GetVertexKineticEnergy() << std::endl;
                    // G4cout << " \t\t : " << aStep->GetTrack()->GetCreatorModelIndex() << std::endl;
                }
            }
        }

    }

    // double time = aStep->GetTrack()->GetGlobalTime();
    // G4EventManager * evtmgr = G4EventManager::GetEventManager();
    // YourEventAction * evt =static_cast<YourEventAction*>(evtmgr->GetUserEventAction());
    // evt->UpdateTime(time);

    // auto & prepos = aStep->GetPreStepPoint()->GetPosition();
    // auto & postpos = aStep->GetPostStepPoint()->GetPosition();
    // auto  avepos = 0.5*(prepos + postpos);
    // double distance_hit_shower_axis = SDutils::Calculate_hitpos_to_shower_axis_distance(avepos,fPrimaryGenerator->direction0, fPrimaryGenerator->position0);
    // if(8*CLHEP::cm < distance_hit_shower_axis)
    //     return false;

    constexpr double birk1      = {2.08029e+18};
    constexpr double birkSlope  = {0.253694};
    constexpr double birkCut    = {0.1};

    double birk_correction = SDutils::getBirkL3(aStep, birk1, birkSlope, birkCut);

    event_energy += edep*birk_correction;
    G4LogicalVolume * lv = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
    energy_accumulator.map[lv] += edep * birk_correction;
    if(1<verbosity)
        std::cout << "\t[" + this->GetName() + "] " << edep/CLHEP::MeV << " MeV" << std::endl;

    return true; // Step properly processed
}

void ECalSD::Initialize(G4HCofThisEvent*) {
    event_energy = 0.0;
    energy_accumulator.Initialize(sensitive_lv);
    event_energy_raw = 0;
    event_energy_raw_gamma = 0;
    event_energy_raw_electron = 0;
    event_energy_raw_neutron = 0;
    event_energy_raw_pion = 0;
    event_energy_raw_proton = 0;
    event_nparticles = 0;
    event_nparticles_gamma = 0;
    event_nparticles_electron = 0;
    event_nparticles_neutron = 0;
    event_nparticles_proton = 0;
    event_nparticles_pion = 0;
    fPDG.clear();
    fModelIndex.clear();
    fElectronCount = 0;
    fElectronE0.clear();
    ResetPrimaryEnergy();
    fModelIndex_electron.clear();
    fGammaPhotoE0.clear();
    fModelIndex_gammaPhoto.clear();

}

void ECalSD::EndOfEvent(G4HCofThisEvent*) {
    if(0<verbosity)
        std::cout << "Total energy [" + this->GetName() + "] " << event_energy/CLHEP::MeV << " MeV" << std::endl;
}
