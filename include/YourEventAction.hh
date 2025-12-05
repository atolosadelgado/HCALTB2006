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
  void AddVisibleEnergy(double e){visible_energy+=e;}

private:
  YourRunAction * fRunAction;
  YourPrimaryGenerator * fPrimaryGenerator;
  double visible_energy;
};

#endif
