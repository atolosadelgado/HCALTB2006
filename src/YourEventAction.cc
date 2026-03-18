#include "YourEventAction.hh"

#include "G4AnalysisManager.hh"


YourEventAction::YourEventAction()
  : G4UserEventAction() {}

void YourEventAction::BeginOfEventAction(const G4Event*) {}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/)
{
    auto* ana = G4AnalysisManager::Instance();
    ana->AddNtupleRow();
}
