#include "YourCaloSD.hh"
#include "YourVCaloResponse.hh"

YourCaloSD::YourCaloSD(std::string name,
           std::unique_ptr<YourVCaloResponse> response,
           std::function<void(G4double)> scorer)
: G4VSensitiveDetector(name),
  fCaloResponse(std::move(response)),
  fEventActionScorer(std::move(scorer)) {}

bool YourCaloSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    return true;
}
