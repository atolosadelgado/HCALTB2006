#include "YourTrackingAction.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4RegionStore.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Version.hh"

#if G4VERSION_NUMBER >= 1100
#include "G4AnalysisManager.hh"
using AnalysisManager = G4AnalysisManager;
#else
#include "G4RootAnalysisManager.hh"
using AnalysisManager = G4RootAnalysisManager;
#endif

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
    trackIDmap[track->GetTrackID()] = {track->GetParticleDefinition(), track->GetVertexKineticEnergy()};
    // if no creator process, return early
    if(0 == track->GetParentID() ) return;
    const G4VProcess * track_creator_process = track->GetCreatorProcess();
    if (nullptr == track_creator_process) return;

    G4RegionStore * regionStore = G4RegionStore::GetInstance();
    auto * fRegionEcal = regionStore->FindOrCreateRegion("EcalRegion");
    auto * fRegionHcal = regionStore->FindOrCreateRegion("HcalRegion");
    auto * trackRegion = track->GetLogicalVolumeAtVertex()->GetRegion();
    if(trackRegion != fRegionEcal && trackRegion != fRegionHcal )
        return;

    auto particleInformation = GetParticleInfo(track);
    int hIDe0 = particleInformation.hIDe0;
    int hIDef = particleInformation.hIDef;
    int hIDtf = particleInformation.hIDtf;
    double e0 = track->GetVertexKineticEnergy();
    double ef = track->GetKineticEnergy();
    double tf = track->GetLocalTime();

    int creatorIndex = 0;

    // MSC and other EM models do not assign modelID...
    if(fUseModelIndex)
    {

#if G4VERSION_NUMBER >= 1100
        creatorIndex = track->GetCreatorModelIndex();
#else
        // before v11.0, only model ID (which corresponds to model Index)
        creatorIndex = track->GetCreatorModelID();
#endif
    }
    else{
        auto procIt = fProcNameId.find(track_creator_process->GetProcessName());
        if(fProcNameId.end() == procIt ){
            creatorIndex = 0;
        }
        else
            creatorIndex = procIt->second + 1;
    }

    auto analysisManager = AnalysisManager::Instance();
    analysisManager->FillH2(hIDe0, std::log10(e0) ,creatorIndex);
    auto it = trackIDmap.find(track->GetParentID());
    if(it != trackIDmap.end()){
        if(G4Neutron::Neutron() == it->second.first)
        {
            analysisManager->FillH2(hIDe0+1, std::log10(e0) ,creatorIndex);
        }
        else if(G4PionMinus::PionMinus() == it->second.first ||
                G4PionPlus::PionPlus() == it->second.first ||
                G4PionZero::PionZero() == it->second.first
                )
        {
            analysisManager->FillH2(hIDe0+2, std::log10(e0) ,creatorIndex);
        }
    }
    analysisManager->FillH2(hIDef, std::log10(ef) ,creatorIndex);
    analysisManager->FillH2(hIDtf, std::log10(tf) ,creatorIndex);

}
