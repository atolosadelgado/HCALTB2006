#ifndef SDutils_hh
#define SDutils_hh

#include <cmath> //isnan

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ios.hh"



namespace SDutils
{
    inline void NaNTrap(const G4Step* aStep) {
        G4Track* currentTrk = aStep->GetTrack();
        double ekin = currentTrk->GetKineticEnergy();
        if (ekin < 0.0) {
            const G4VPhysicalVolume* pCurrentVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
            G4cerr << "Negative kinetic energy Ekin(MeV)=" << ekin / CLHEP::MeV << " of "
                                                << currentTrk->GetDefinition()->GetParticleName()
                                                << " trackID= " << currentTrk->GetTrackID() << " inside "
                                                << pCurrentVol->GetName() << G4endl;
            currentTrk->SetKineticEnergy(0.0);
        }
        const G4ThreeVector& currentPos = currentTrk->GetPosition();
        double xyz = currentPos.x() + currentPos.y() + currentPos.z();
        const G4ThreeVector& currentMom = currentTrk->GetMomentum();
        xyz += currentMom.x() + currentMom.y() + currentMom.z();

        if ( std::isnan(xyz)) {
            const G4VPhysicalVolume* pCurrentVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
            G4cerr << "SensitiveDetector\t" << "NaN detected for trackID= " << currentTrk->GetTrackID() << " inside "
                                                << pCurrentVol->GetName() << G4endl;
            G4Exception("SensitiveDetector::NaNTrap()", "sd01", FatalException, "corrupted event or step");
        }
    }

    // from CMSSW, this function is a method of CaloSD
    // https://github.com/cms-sw/cmssw/blob/2c3832ae6100c8b0c791afaa038a0de475cde39a/SimG4CMS/Calo/src/CaloSD.cc#L738
    // to be applied only if (isItScintillator(mat))
    inline double getAttenuation(const G4Step* aStep, double birk1, double birk2, double birk3) {
        double weight = 1.;
        double charge = aStep->GetPreStepPoint()->GetCharge();
        double length = aStep->GetStepLength();

        if (charge != 0. && length > 0.) {
            double density = aStep->GetPreStepPoint()->GetMaterial()->GetDensity();
            double dedx = aStep->GetTotalEnergyDeposit() / length;
            double rkb = birk1 / density;
            double c = birk2 * rkb * rkb;
            if (std::abs(charge) >= 2.)
            rkb /= birk3;  // based on alpha particle data
            weight = 1. / (1. + rkb * dedx + c * dedx * dedx);
        }
        return weight;
    }
    // from CMSSW, this function is a method of ECalSD,
    // https://github.com/cms-sw/cmssw/blob/2c3832ae6100c8b0c791afaa038a0de475cde39a/SimG4CMS/Calo/src/ECalSD.cc#L479
    inline double getBirkL3(const G4Step* aStep, double birk1, double birkSlope, double birkCut) {
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

};

#endif
