#ifndef YOURRUNACTION_HH
#define YOURRUNACTION_HH

#include "G4UserRunAction.hh"

#include <string>

class HistoEnergyResponse;
class YourPrimaryGenerator;
class YourSteppingAction;

class YourRunAction : public G4UserRunAction {

  public:

    YourRunAction(std::string ofilename);
    virtual ~YourRunAction();

    void   BeginOfRunAction(const G4Run* run) override;
    void   EndOfRunAction(const G4Run* run) override;

    void SetPrimaryGenerator(YourPrimaryGenerator * g);
    void SetSteppingAction(YourSteppingAction * s);

    void FillEventEnergy(double ecal_energy, double hcal_energy);

    std::string _ofilename;
    HistoEnergyResponse * fHenergyResponse;
    YourPrimaryGenerator * fPrimaryGenerator;
    YourSteppingAction * fSteppingAction;

    int verbosity = 0;
};

#endif
