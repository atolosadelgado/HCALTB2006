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

  void UpdateCaloEfluxOut(G4double eflux_out){fCaloEfluxOut.fTotalEnergy+=eflux_out;}
  void UpdateCaloEfluxIn (G4double eflux_in ){fCaloEfluxIn.fTotalEnergy +=eflux_in ;}

  void SetCaloEfluxOutNtupleID(G4int id){fCaloEfluxOut.fNtupleId = id; }
  void SetCaloEfluxInNtupleID (G4int id){fCaloEfluxIn.fNtupleId  = id; }

private:

  // pointer to G4 event information
  // initialized in BeginOfEventAction
  YourEventInfo * fEventInfo;

  // object storing map LV -> layer ID
  // set by BeginOfRunAction
  YourLayerInfo fLayerInfo;

  // vectors are actually owned by run action
  // the pointer is used to access them from event action
  std::vector<double> * fEnergyProfile;
  std::vector<double> * fRadiusProfile;

  // helper structure to score energy
  // in ECAl/HCAL regions and keep ntuple ID
  // id setup in BeginOfRunAction
  struct YourGlobalAccum
  {
    G4Region * fRegion = nullptr;
    G4double fTotalEnergy = 0;
    G4int    fNtupleId = 0;
  };
  YourGlobalAccum fEcalAccum;
  YourGlobalAccum fHcalAccum;
  YourGlobalAccum fCaloEfluxOut;
  YourGlobalAccum fCaloEfluxIn;
};

#endif
