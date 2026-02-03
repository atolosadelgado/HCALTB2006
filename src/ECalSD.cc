#include "ECalSD.h"

#include <iostream>

G4bool ECalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    double edep = getEnergyDeposit(aStep);
    if (edep <= 0.) return false;
    event_energy += edep;
    if(1<verbosity)
        std::cout << "\t[" + this->GetName() + "] " << edep/CLHEP::MeV << " MeV" << std::endl;

    return true; // Step properly processed
}

double ECalSD::getEnergyDeposit(G4Step* aStep) {
    return aStep->GetTotalEnergyDeposit();
}

void ECalSD::Initialize(G4HCofThisEvent*) {
    event_energy = 0.0;
}

void ECalSD::EndOfEvent(G4HCofThisEvent*) {
    if(0<verbosity)
        std::cout << "Total energy [" + this->GetName() + "] " << event_energy/CLHEP::MeV << " MeV" << std::endl;
}


