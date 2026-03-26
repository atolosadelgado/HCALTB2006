#include "YourTrackingAction.hh"

#include "G4Track.hh"
#include "G4Neutron.hh"
#include "G4EventManager.hh"
#include "G4AnalysisManager.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"

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

void YourTrackingAction::PostUserTrackingAction(const G4Track* track)
{
    const G4VProcess * track_creator_process = track->GetCreatorProcess();
    // if no creator process, return early
    if (nullptr == track_creator_process) return;

    auto particleInformation = GetParticleInfo(track);
    int hIDe0 = particleInformation.hIDe0;
    int hIDef = particleInformation.hIDef;
    int hIDtf = particleInformation.hIDtf;
    double e0 = track->GetVertexKineticEnergy();
    double ef = track->GetKineticEnergy();
    double tf = track->GetLocalTime();
    // MSC and other EM models do not assign modelID...
    // int modelIndex = track->GetCreatorModelIndex();
    auto * pmanager = track->GetDynamicParticle()->GetParticleDefinition()->GetProcessManager();
    G4ProcessVector * pv = pmanager->GetProcessList();
    int pindex = 0;
    for(int i = 0; i < pmanager->GetProcessListLength(); ++i)
    {
        if( track_creator_process->GetProcessName() == (*pv)[i]->GetProcessName() ){
            pindex = i;
            break;
        }
    }

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(hIDe0, std::log10(e0) ,pindex);
    analysisManager->FillH2(hIDef, std::log10(ef) ,pindex);
    analysisManager->FillH2(hIDtf, std::log10(tf) ,pindex);
}
