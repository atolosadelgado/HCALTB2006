#ifndef CaloSD_h
#define CaloSD_h

#include <string>

#include "SensitiveCaloDetector.h"

class CaloSD : public SensitiveCaloDetector {
public:
    CaloSD(std::string fname):SensitiveCaloDetector(fname){}
    ~CaloSD() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override = 0;
};

#endif
