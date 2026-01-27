#ifndef ECalSD_h
#define ECalSD_h

#include <string>

#include "CaloSD.h"

class ECalSD : public CaloSD {
public:
    ECalSD(std::string fname):CaloSD(fname) { std::cout << "New EcalSD named " << fname.c_str() << std::endl;}
    ~ECalSD() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) final {return false;};
};

#endif
