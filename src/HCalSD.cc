#include "HCalSD.h"
#include "SDutils.hh"

#include <iostream>

G4bool HCalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    double edep = aStep->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;
        // # Values of Birks constants from NIM 80 (1970) 239-244:
        // # as implemented in Geant3 required correction due to
        // # biased computation of enery deposition
        // BirkC3                    = cms.double(1.75),
        // BirkC2                    = cms.double(0.142),
        // BirkC1                    = cms.double(0.0060),
    // double bunit = (CLHEP::g / (CLHEP::MeV * CLHEP::cm2));
    // 3.74491e+17 = 0.0060 * bunit
    double birk_correction = SDutils::getAttenuation(aStep, 3.74491e+17, 0.142, 1.75);
    std::string lvname = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
    double Wt0_correction = ("HBScintillatorLayer0In1" == lvname) || ("HBScintillatorLayer0In2" == lvname) ? 0.41 : 1.0;
    event_energy += (edep * birk_correction * Wt0_correction);
    event_energy_birk += (edep * birk_correction);
    event_energy_0wt += (edep * Wt0_correction);
    // G4cout << "\t+++ New step: " << edep << "\t" << birk_correction << "\t" << Wt0_correction << std::endl;
    if(1<verbosity)
        std::cout << "\t[" + this->GetName() + "] " << edep/CLHEP::MeV << " MeV" << std::endl;

    return true; // Step properly processed
}

double HCalSD::getEnergyDeposit(G4Step* aStep) {
    return aStep->GetTotalEnergyDeposit();
}

void HCalSD::Initialize(G4HCofThisEvent*) {
    event_energy = 0.0;
    event_energy_birk = 0.0;
    event_energy_0wt = 0.0;
}

void HCalSD::EndOfEvent(G4HCofThisEvent*) {
    if(0<verbosity)
        std::cout << "Total energy [" + this->GetName() + "] " << event_energy/CLHEP::MeV << " MeV" << std::endl;
}
