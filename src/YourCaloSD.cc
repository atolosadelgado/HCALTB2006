#include "YourCaloSD.hh"
#include "YourVCaloResponse.hh"
#include "YourClusterCut.hh"

#include "G4Version.hh"

#if G4VERSION_NUMBER >= 1100
#include "G4AnalysisManager.hh"
using AnalysisManager = G4AnalysisManager;
#else
#include "G4RootAnalysisManager.hh"
using AnalysisManager = G4RootAnalysisManager;
#endif

YourCaloSD::YourCaloSD(std::string name,
                       std::unique_ptr<YourVCaloResponse> response,
                       std::unique_ptr<YourClusterCut> clustercut  )
: G4VSensitiveDetector(name),
  fCaloResponse(std::move(response)),
  fClusterCut(std::move(clustercut)){}

bool YourCaloSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // early return if hit away
    if (fClusterCut && !fClusterCut->IsInside(aStep)) return false;
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double corr = fCaloResponse->GetCorrection(aStep);
    fTotalEnergy += edep * corr;
    fTotalEnergy_raw += edep;
    return true;
}

void YourCaloSD::Initialize(G4HCofThisEvent*)
{
    fTotalEnergy = 0;
    fTotalEnergy_raw = 0;
}

void YourCaloSD::EndOfEvent(G4HCofThisEvent*)
{
    auto* ana = AnalysisManager::Instance();
    ana->FillNtupleDColumn(fNTupleColumnID, fTotalEnergy);
    ana->FillNtupleDColumn(fNTupleColumnID+1, fTotalEnergy_raw);
    if(0<fVerbosity)
        G4cout << "Total energy in <" + GetName() << "> : " << fTotalEnergy / CLHEP::MeV << " MeV" << G4endl;
}
