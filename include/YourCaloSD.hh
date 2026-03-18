#ifndef YOURCALOSD_HH
#define YOURCALOSD_HH

#include <string>
#include <memory>
#include <functional>

#include "G4VSensitiveDetector.hh"

class YourVCaloResponse;

/* class YourCaloSD
 * reimplements CMSSW classes
 * EcalSD/HcalSD -> CaloSD -> SensitiveCaloDetector -> SensitiveDetector
 * in one single class. Birk (and 0wt) correction encapsulated in YourCaloResponse class
 */
class YourCaloSD : public G4VSensitiveDetector {
public:
    YourCaloSD(std::string name,
           std::unique_ptr<YourVCaloResponse> response,
           std::function<void(G4double)> eventActionScorer);

    ~YourCaloSD() override = default;

    // void Initialize(G4HCofThisEvent * ) override;
    // void EndOfEvent(G4HCofThisEvent * ) override;

    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;

private:
    int fVerbosity = {0};
    std::unique_ptr<YourVCaloResponse> fCaloResponse;
    std::function<void(G4double)> fEventActionScorer;
};

#endif
