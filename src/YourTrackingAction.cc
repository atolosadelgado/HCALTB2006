#include "YourTrackingAction.hh"

#include "G4Track.hh"
#include "G4Neutron.hh"
#include "G4EventManager.hh"
#include "G4AnalysisManager.hh"

#include "YourEventAction.hh"

YourTrackingAction::YourTrackingAction()
: G4UserTrackingAction()
{}

YourTrackingAction::~YourTrackingAction()
{}

const YourParticleInfo & YourTrackingAction::GetParticleInfo(const G4Track* track) const
{
    int pdgID = track->GetParticleDefinition()->GetPDGEncoding();
    auto particleInformation = fParticleInfoMap.find(pdgID);
    if( fParticleInfoMap.end() == particleInformation )
        return fParticleInfoMap.at(YourParticleInfo::PDG_OTHER);
    else
        return particleInformation->second;
}


void YourTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    auto particleInformation = GetParticleInfo(track);
    int hID = particleInformation.hIDe0;
    double e0 = track->GetVertexKineticEnergy();
    int modelIndex = track->GetCreatorModelIndex();

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(hID, std::log10(e0) ,modelIndex);
}


void YourTrackingAction::PostUserTrackingAction(const G4Track* track)
{
    auto particleInformation = GetParticleInfo(track);
    int hIDef = particleInformation.hIDef;
    int hIDtf = particleInformation.hIDtf;
    double ef = track->GetKineticEnergy();
    double tf = track->GetLocalTime();
    int modelIndex = track->GetCreatorModelIndex();

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(hIDef, std::log10(ef) ,modelIndex);
    analysisManager->FillH2(hIDtf, std::log10(tf) ,modelIndex);
    if(ef<1e-15)
        G4cout << "Warning, final energy is " << ef << std::endl;
}
