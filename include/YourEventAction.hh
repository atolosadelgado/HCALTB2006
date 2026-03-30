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

  void SetEnergyProfileVector(std::vector<double> * EnergyProfileVector){fEnergyProfile=EnergyProfileVector;}
  void SetRadiusProfileVector(std::vector<double> * RadiusProfileVector){fRadiusProfile=RadiusProfileVector;}

  void InitializeRegionDefinition();
  void UpdateTotalEnergyPerRegion(G4Region * rg, G4double edep);
  void SetEcalTotalEnergyNtupleID(G4int id){fEcalAccum.fNtupleId = id; }
  void SetHcalTotalEnergyNtupleID(G4int id){fHcalAccum.fNtupleId = id; }

  // hot functions, no checking if null ptr
  G4ThreeVector & GetPrimaryVertexPos(){return fEventInfo->primaryPos;}
  G4ThreeVector & GetPrimaryVertexDir(){return fEventInfo->primaryDir;}

private:

  // pointer to G4 event information
  // initialized in BeginOfEventAction
  YourEventInfo * fEventInfo;

  // object storing map LV -> layer ID
  // set by BeginOfRunAction
  YourLayerInfo fLayerInfo;

  // vectors owned by run action
  std::vector<double> * fEnergyProfile;
  std::vector<double> * fRadiusProfile;

  // helper structure to score energy
  // in ECAl/HCAL regions and keep ntuple ID
  // id setup in BeginOfRunAction
  struct YourGlobalAccum
  {
    G4Region * fRegion;
    G4double fTotalEnergy;
    G4int    fNtupleId;
  };
  YourGlobalAccum fEcalAccum;
  YourGlobalAccum fHcalAccum;
};

#endif
