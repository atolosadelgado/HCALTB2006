#ifndef YOUREMSATURATION
#define YOUREMSATURATION

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include "G4Step.hh"
#include "G4Material.hh"

class YourVEMSaturation
{
public:
    virtual ~YourVEMSaturation() = default;
    virtual double GetEnergy(const G4Step * theStep) = 0;
};

class YourEMSaturationNone : public YourVEMSaturation
{
public:
    double GetEnergy(const G4Step * theStep) override {
        return theStep->GetTotalEnergyDeposit();
    };
};

class YourEMSaturationG4Birk : public YourVEMSaturation
{
public:
    double GetEnergy(const G4Step * theStep) override {
        return G4LossTableManager::Instance()->EmSaturation()->VisibleEnergyDepositionAtAStep(theStep);
    }
};

/// CMS includes several weights or calibrating factors, this class implements only Birks
/// currently hardcoded Birc parameters for HCAL, to be extended for ECAL
class YourEMSaturationCMSBirk : public YourVEMSaturation
{
public:
    double GetEnergy(const G4Step * theStep) override {return theStep->GetTotalEnergyDeposit() * getAttenuation(theStep);}
    // function from CMSSW,
    // https://github.com/cms-sw/cmssw/blob/9eb4c9a0a231e5395c9f9b95bb798ee39416bdaa/SimG4CMS/Calo/src/CaloSD.cc#L738
    double getAttenuation(const G4Step* aStep) const {
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
    // # Values of Birks constants from NIM 80 (1970) 239-244:
    // # as implemented in Geant3 required correction due to
    // # biased computation of enery deposition
    // BirkC3                    = cms.double(1.75),
    // BirkC2                    = cms.double(0.142),
    // BirkC1                    = cms.double(0.0060),
    // https://github.com/cms-sw/cmssw/blob/9eb4c9a0a231e5395c9f9b95bb798ee39416bdaa/SimG4Core/Application/python/g4SimHits_cfi.py#L430
    double birk1 = {0.0060};
    double birk2 = {0.142};
    double birk3 = {1.75};
};


#endif
