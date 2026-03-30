#include "YourTrackingAction.hh"

#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4VProcess.hh"
#include "G4RegionStore.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"

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

    // MSC and other EM models do not assign modelID...
    // int modelIndex = track->GetCreatorModelIndex();

    int pindex = 0;
    auto procIt = fProcNameId.find(track_creator_process->GetProcessName());
    if(fProcNameId.end() == procIt ){
        pindex = 0;
    }
    else
        pindex = procIt->second + 1;

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(hIDe0, std::log10(e0) ,pindex);
    if(auto it = trackIDmap.find(track->GetParentID()); it != trackIDmap.end()){
        if(G4Neutron::Neutron() == it->second.first)
        {
            analysisManager->FillH2(hIDe0+1, std::log10(e0) ,pindex);
        }
        else if(G4PionMinus::PionMinus() == it->second.first ||
                G4PionPlus::PionPlus() == it->second.first ||
                G4PionZero::PionZero() == it->second.first
                )
        {
            analysisManager->FillH2(hIDe0+2, std::log10(e0) ,pindex);
        }
    }
    analysisManager->FillH2(hIDef, std::log10(ef) ,pindex);
    analysisManager->FillH2(hIDtf, std::log10(tf) ,pindex);

}
