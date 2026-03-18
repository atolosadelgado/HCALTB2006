#ifndef YOURVCALORESPONSE_HH
#define YOURVCALORESPONSE_HH

class G4Step;

/* YourVCaloResponse is an interface to calculate correction to energy deposited in the step
 * TODO: extend it to account for time/radial calculates
 */
class YourVCaloResponse
{
public:
    virtual ~YourVCaloResponse() = default;
    virtual double GetCorrection(const G4Step * theStep) = 0;

protected:
    // from CMSSW, this function is a method of CaloSD
    // https://github.com/cms-sw/cmssw/blob/2c3832ae6100c8b0c791afaa038a0de475cde39a/SimG4CMS/Calo/src/CaloSD.cc#L738
    // to be applied only if (isItScintillator(mat))
    double getAttenuation(const G4Step* aStep, double birk1, double birk2, double birk3);

    // from CMSSW, this function is a method of ECalSD,
    // https://github.com/cms-sw/cmssw/blob/2c3832ae6100c8b0c791afaa038a0de475cde39a/SimG4CMS/Calo/src/ECalSD.cc#L479
    double getBirkL3(const G4Step* aStep, double birk1, double birkSlope, double birkCut);
};

#endif
