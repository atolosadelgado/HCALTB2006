#ifndef YOURECALRESPONSE_HH
#define YOURECALRESPONSE_HH

#include "YourVCaloResponse.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"

/* YourEcalResponse calculates correction to energy deposited in the step
 * TODO: extend it to account for time/radial calculates
 */
class YourEcalResponse : public YourVCaloResponse
{
public:
    double GetCorrection(const G4Step * theStep) override {
        constexpr double birk1      = {2.08029e+18};
        constexpr double birkSlope  = {0.253694};
        constexpr double birkCut    = {0.1};
        double birk_correction = getBirkL3(theStep, birk1, birkSlope, birkCut);
        return birk_correction;
    };
};

#endif
