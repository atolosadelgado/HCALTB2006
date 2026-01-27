#ifndef SimpleCaloSD_h
#define SimpleCaloSD_h

#include <string>
#include <iostream>

#include "G4VSensitiveDetector.hh"

class SimpleCaloSD : public G4VSensitiveDetector {
public:
    SimpleCaloSD(std::string fname):G4VSensitiveDetector(fname){ std::cout << "New EcalSD named " << fname.c_str() << std::endl; }
    ~SimpleCaloSD() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override { return false;}
};

#endif
