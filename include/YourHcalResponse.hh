#ifndef YOURHCALRESPONSE_HH
#define YOURHCALRESPONSE_HH

#include "YourVCaloResponse.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"

/* YourHcalResponse calculates correction to energy deposited in the step
 * TODO: extend it to account for time/radial calculates
 */
class YourHcalResponse : public YourVCaloResponse
{
public:
    double GetCorrection(const G4Step * aStep) override {
        // # Values of Birks constants from NIM 80 (1970) 239-244:
        // # as implemented in Geant3 required correction due to
        // # biased computation of enery deposition
        // BirkC3                    = cms.double(1.75),
        // BirkC2                    = cms.double(0.142),
        // BirkC1                    = cms.double(0.0060),
        // double bunit = (CLHEP::g / (CLHEP::MeV * CLHEP::cm2));
        // 3.74491e+17 = 0.0060 * bunit
        double birk_correction = getAttenuation(aStep, 3.74491e+17, 0.142, 1.75);
        G4LogicalVolume * lv = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
        std::string lvname = lv->GetName();
        // this correction is used to apply a global correction factor, is copied from CMSSW and to keep it simple
        // but scaling the energy like this it is not physical
        double Wt0_correction = ("HBScintillatorLayer0In1" == lvname) || ("HBScintillatorLayer0In2" == lvname) ? 0.41 : 1.0;
        double correction = birk_correction * Wt0_correction;
        return correction;
    }
};

#endif
