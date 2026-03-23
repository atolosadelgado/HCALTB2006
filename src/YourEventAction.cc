#include "YourEventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"


YourEventAction::YourEventAction()
  : G4UserEventAction() {}

void YourEventAction::BeginOfEventAction(const G4Event* evt) {
  // reset values in the energy profile vector to zero
  std::fill(fEnergyProfile.begin(), fEnergyProfile.end(), 0);
  std::fill(fRadiusProfile.begin(), fRadiusProfile.end(), 0);
  fEventInfo = dynamic_cast<YourEventInfo*>(evt->GetUserInformation());
}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/)
{
    this->FinalizeProfileHistograms();
    auto* ana = G4AnalysisManager::Instance();
    ana->AddNtupleRow();
}


void YourEventAction::InitializeProfileHistograms(const YourLayerInfo & linfo){
  fLayerInfo = linfo;
  // initialize energy profile vector to the number of layers
  fEnergyProfile = std::vector<double>( fLayerInfo.GetMaxLayerNumber(), 0.0 );
  fRadiusProfile = std::vector<double>( fLayerInfo.GetMaxLayerNumber(), 0.0 );
}

void YourEventAction::UpdateProfileHistograms(G4LogicalVolume* lv, double edep, double radius)
{
  auto l = fLayerInfo.GetLayer(lv);
  // if l is NO_LAYER means that the volume is not in the database
  if(YourLayerInfo::NO_LAYER != l){
    fEnergyProfile[l]+=edep;
    fRadiusProfile[l]+=edep*radius;
  }
}

void YourEventAction::FinalizeProfileHistograms()
{
  // normalize radius*edep by total energy in that layer
  for(auto l = 0; l < fEnergyProfile.size(); ++l)
    fRadiusProfile[l]/=fEnergyProfile[l];
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

std::vector<double> & YourEventAction::GetRadiusProfileVector()
{
    if(0 == fRadiusProfile.size() )
    {
      G4ExceptionDescription msg;
      msg << "Radius profile vector is empty" << G4endl;
      G4Exception("YourEventAction::GetEnergyProfileVector", "Code002", JustWarning, msg);
    }
    return fRadiusProfile;
}
