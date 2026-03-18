#include "YourVCaloResponse.hh"

#include "G4Step.hh"

inline double YourVCaloResponse::getAttenuation(const G4Step* aStep, double birk1, double birk2, double birk3) {
    double weight = 1.;
    double charge = aStep->GetPreStepPoint()->GetCharge();
    double length = aStep->GetStepLength();

    if (charge != 0. && length > 0.) {
        double density = aStep->GetPreStepPoint()->GetMaterial()->GetDensity();
        double dedx = aStep->GetTotalEnergyDeposit() / length;
        double rkb = birk1 / density;
        double c = birk2 * rkb * rkb;
        if (std::fabs(charge) >= 2.)
        rkb /= birk3;  // based on alpha particle data
        weight = 1. / (1. + rkb * dedx + c * dedx * dedx);
    }
    return weight;
}

// from CMSSW, this function is a method of ECalSD,
// https://github.com/cms-sw/cmssw/blob/2c3832ae6100c8b0c791afaa038a0de475cde39a/SimG4CMS/Calo/src/ECalSD.cc#L479
inline double YourVCaloResponse::getBirkL3(const G4Step* aStep, double birk1, double birkSlope, double birkCut) {
    double weight = 1.;
    const G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    double charge = preStepPoint->GetCharge();

    if (charge != 0. && aStep->GetStepLength() > 0.) {
        const G4Material* mat = preStepPoint->GetMaterial();
        double density = mat->GetDensity();
        double dedx = aStep->GetTotalEnergyDeposit() / aStep->GetStepLength();
        double rkb = birk1 / density;
        if (dedx > 0) {
        weight = 1. - birkSlope * log(rkb * dedx);
        if (weight < birkCut)
            weight = birkCut;
        else if (weight > 1.)
            weight = 1.;
        }

    }
    return weight;
}
