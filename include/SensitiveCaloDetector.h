#ifndef SensitiveCaloDetector_h
#define SensitiveCaloDetector_h

#include <string>

#include "SensitiveDetector.h"

class SensitiveCaloDetector : public SensitiveDetector {
public:
    SensitiveCaloDetector(std::string fname):SensitiveDetector(fname){}
    ~SensitiveCaloDetector() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override = 0;
};

#endif
