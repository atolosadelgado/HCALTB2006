#ifndef SensitiveDetector_h
#define SensitiveDetector_h

#include <string>

#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(std::string fname):G4VSensitiveDetector(fname){}
    ~SensitiveDetector() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override = 0;
};

#endif
