#ifndef YOURRUNACTION_HH
#define YOURRUNACTION_HH

#include "G4UserRunAction.hh"

#include <string>

class YourPrimaryGenerator;
class YourSteppingAction;
class YourInputArgs;

class YourRunAction : public G4UserRunAction {

  public:

    YourRunAction(std::string ofilename, const YourInputArgs * args);
    virtual ~YourRunAction();

    void   BeginOfRunAction(const G4Run* run) override;
    void   EndOfRunAction(const G4Run* run) override;

    void FillEventEnergy(double ecal_energy, double hcal_energy);

    void ConstructOutputTree();
    void BeginOutputTree();
    void EndOutputTree();
    void FillOutputTree(double ecal_eresponse, double hcal_eresponse);

    void SetSteppingAction(YourSteppingAction * s) {fSteppingAction = s;}

    std::string _ofilename;
    const YourInputArgs * fInputArgs;
    YourSteppingAction * fSteppingAction;

    int verbosity = 0;
private:
    inline const YourPrimaryGenerator* GetPrimaryGenerator();
};

#endif
