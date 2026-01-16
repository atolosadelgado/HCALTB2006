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
  void RegisterTime(double t);
  // this function stores the time of first hit, and if the
  // difference of the subsequent times is lower than fTimeMax,
  // it will return true. fTimeMax is hardcoded in constructor
  bool IsTimeWithinEventTimeWindow(double t);

private:
  YourRunAction * fRunAction;
  YourPrimaryGenerator * fPrimaryGenerator;
  double visible_energy_ecal;
  double visible_energy_hcal;
  double time_first_hit;
  double time_last_hit;
  double fTimeMax;
};

#endif
