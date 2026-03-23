#include "YourEventAction.hh"

#include "G4AnalysisManager.hh"


YourEventAction::YourEventAction()
  : G4UserEventAction() {}

void YourEventAction::BeginOfEventAction(const G4Event*) {
  // reset values in the energy profile vector to zero
  std::fill(fEnergyProfile.begin(), fEnergyProfile.end(), 0);
}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/)
{
    auto* ana = G4AnalysisManager::Instance();
    ana->AddNtupleRow();
}

std::vector<double> & YourEventAction::GetEnergyProfileVector()
{
    if(0 == fEnergyProfile.size() )
    {
      G4ExceptionDescription msg;
      msg << "Energy profile vector is empty" << G4endl;
      G4Exception("YourEventAction::GetEnergyProfileVector", "Code001", JustWarning, msg);
    }
    return fEnergyProfile;
}

void YourEventAction::SetLayerInfo(const YourLayerInfo & linfo){
  fLayerInfo = linfo;
  // initialize energy profile vector to the number of layers
  fEnergyProfile = std::vector<double>( fLayerInfo.GetMaxLayerNumber(), 0.0 );
}

void YourEventAction::AddEnergy(G4LogicalVolume* lv, double edep)
{
  auto l = fLayerInfo.GetLayer(lv);
  // if l is NO_LAYER means that the volume is not in the database
  if(YourLayerInfo::NO_LAYER != l)
    fEnergyProfile[l]+=edep;
}
