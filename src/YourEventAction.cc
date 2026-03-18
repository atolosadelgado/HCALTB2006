#include "YourEventAction.hh"
#include "YourRunAction.hh"
#include "YourPrimaryGenerator.hh"

#include "G4AnalysisManager.hh"
#include "G4Material.hh"

YourEventAction::YourEventAction(YourRunAction * myRunAction, YourPrimaryGenerator * gen)
  : G4UserEventAction(),
  fRunAction(myRunAction), fPrimaryGenerator(gen), fTimeMax(50*CLHEP::ns) {}

void YourEventAction::BeginOfEventAction(const G4Event*) {
  fEdepEcalPerEvt = 12;
  fEdepHcalPerEvt = 34;
}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/)
{
    auto* ana = G4AnalysisManager::Instance();
    ana->FillNtupleDColumn(fIdNtuple_EcalEnergy, fEdepEcalPerEvt );
    ana->FillNtupleDColumn(fIdNtuple_HcalEnergy, fEdepHcalPerEvt );
    ana->AddNtupleRow();
}
