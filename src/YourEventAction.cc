#include "YourEventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4RegionStore.hh"


YourEventAction::YourEventAction()
  : G4UserEventAction() {}

void YourEventAction::BeginOfEventAction(const G4Event* evt) {
  // reset values in the energy profile vector to zero
  std::fill(fEnergyProfile->begin(), fEnergyProfile->end(), 0);
  std::fill(fRadiusProfile->begin(), fRadiusProfile->end(), 0);
  fEventInfo = dynamic_cast<YourEventInfo*>(evt->GetUserInformation());
  fEcalAccum.fTotalEnergy = 0.0;
  fHcalAccum.fTotalEnergy = 0.0;
  fCaloEfluxOut.fTotalEnergy = 0.0;
  fCaloEfluxIn.fTotalEnergy  = 0.0;
}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/)
{
    this->FinalizeProfileHistograms();
    auto* ana = G4AnalysisManager::Instance();

    ana->FillNtupleDColumn(fEcalAccum.fNtupleId, fEcalAccum.fTotalEnergy );
    ana->FillNtupleDColumn(fHcalAccum.fNtupleId, fHcalAccum.fTotalEnergy );

    ana->FillNtupleDColumn(fCaloEfluxOut.fNtupleId, fCaloEfluxOut.fTotalEnergy);
    ana->FillNtupleDColumn(fCaloEfluxIn.fNtupleId,  fCaloEfluxIn.fTotalEnergy );

    ana->AddNtupleRow();
}


void YourEventAction::InitializeProfileHistograms(const YourLayerInfo & linfo){
  fLayerInfo = linfo;
}

void YourEventAction::UpdateProfileHistograms(G4LogicalVolume* lv, double edep, double radius)
{
  auto l = fLayerInfo.GetLayer(lv);
  // // if l is NO_LAYER means that the volume is not in the database
  if(YourLayerInfo::NO_LAYER != l){
    fEnergyProfile->operator[](l)+=edep;
    fRadiusProfile->operator[](l)+=edep*radius;
  }
}

void YourEventAction::FinalizeProfileHistograms()
{
  // normalize radius*edep by total energy in that layer
  for(std::size_t l = 0; l < fEnergyProfile->size(); ++l)
    fRadiusProfile->operator[](l)/=fEnergyProfile->operator[](l);
}

void YourEventAction::InitializeRegionDefinition()
{
  G4RegionStore * regionStore = G4RegionStore::GetInstance();
  // FIXME: names are hardcoded, valid only for GDML HCAL TB 2006 geometry!
  fEcalAccum.fRegion = regionStore->FindOrCreateRegion("EcalRegion");
  fHcalAccum.fRegion = regionStore->FindOrCreateRegion("HcalRegion");
}

void YourEventAction::UpdateTotalEnergyPerRegion(G4Region* rg, G4double edep)
{
  if(fHcalAccum.fRegion == rg) fHcalAccum.fTotalEnergy+=edep;
  else if(fEcalAccum.fRegion == rg) fEcalAccum.fTotalEnergy+=edep;
}
