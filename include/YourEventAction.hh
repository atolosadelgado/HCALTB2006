#ifndef YOUREVENTACTION_HH
#define YOUREVENTACTION_HH

#include "G4UserEventAction.hh"

#include "YourLayerInfo.hh"

#include <vector>

class YourEventAction : public G4UserEventAction {
public:

  YourEventAction();
  ~YourEventAction() override = default;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

  void SetLayerInfo(const YourLayerInfo & linfo);

  std::vector<double> & GetEnergyProfileVector();

  void AddEnergy(G4LogicalVolume * lv, double edep);

private:
  YourLayerInfo fLayerInfo;
  std::vector<double> fEnergyProfile;

};

#endif
