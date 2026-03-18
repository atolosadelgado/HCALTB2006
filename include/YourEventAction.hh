#ifndef YOUREVENTACTION_HH
#define YOUREVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

class YourPrimaryGenerator;
class YourRunAction;

class YourEventAction : public G4UserEventAction {
public:

  YourEventAction(YourRunAction * myRunAction, YourPrimaryGenerator* gen);
  ~YourEventAction() override = default;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

  void SetIdNtuple_EcalEnergy(G4int id){fIdNtuple_EcalEnergy = id;}
  void SetIdNtuple_HcalEnergy(G4int id){fIdNtuple_HcalEnergy = id;}

  // add energy, natural units
  void AddVisibleEnergyECAL(G4double e){fEdepEcalPerEvt+=e;}
  void AddVisibleEnergyHCAL(G4double e){fEdepHcalPerEvt+=e;}

private:
  YourRunAction * fRunAction;
  YourPrimaryGenerator * fPrimaryGenerator;
  G4double fEdepEcalPerEvt;
  G4double fEdepHcalPerEvt;

  G4int fIdNtuple_EcalEnergy = {-1};
  G4int fIdNtuple_HcalEnergy = {-1};

  // TODO: add time cut
  G4double fTimeMax;
  // TODO: add XY (Radial) cut
  // fPrimaryGenerator -> for the shower axis
  // fRadiusMax -> different for ECAL and HCAL!!
};

#endif
