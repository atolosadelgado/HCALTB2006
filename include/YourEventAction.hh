#ifndef YOUREVENTACTION_HH
#define YOUREVENTACTION_HH

#include "G4UserEventAction.hh"

class YourPrimaryGenerator;
class YourRunAction;

class YourEventAction : public G4UserEventAction {
public:

  YourEventAction(YourRunAction * myRunAction, YourPrimaryGenerator* gen);
  ~YourEventAction() override;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

  // add energy, natural units
  void AddVisibleEnergyECAL(double e){visible_energy_ecal+=e;}
  void AddVisibleEnergyHCAL(double e){visible_energy_hcal+=e;}

private:
  YourRunAction * fRunAction;
  YourPrimaryGenerator * fPrimaryGenerator;
  double visible_energy_ecal;
  double visible_energy_hcal;
};

#endif
