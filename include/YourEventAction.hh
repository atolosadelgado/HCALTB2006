#ifndef YOUREVENTACTION_HH
#define YOUREVENTACTION_HH

#include "G4UserEventAction.hh"

#include "YourLayerInfo.hh"
#include "YourEventInfo.hh"

#include <vector>

class YourEventInfo;

class YourEventAction : public G4UserEventAction {
public:

  YourEventAction();
  ~YourEventAction() override = default;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

  void InitializeProfileHistograms(const YourLayerInfo & linfo);

  void UpdateProfileHistograms(G4LogicalVolume * lv, double edep, double radius);

  void FinalizeProfileHistograms();

  std::vector<double> & GetEnergyProfileVector();

  std::vector<double> & GetRadiusProfileVector();

  // hot functions, no checking if null ptr
  G4ThreeVector & GetPrimaryVertexPos(){return fEventInfo->primaryPos;}
  G4ThreeVector & GetPrimaryVertexDir(){return fEventInfo->primaryDir;}

private:
  YourLayerInfo fLayerInfo;
  std::vector<double> fEnergyProfile;
  std::vector<double> fRadiusProfile;
  YourEventInfo * fEventInfo;

};

#endif
