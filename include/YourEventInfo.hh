#ifndef YOUREVENTINFO_HH
#define YOUREVENTINFO_HH

#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"

class YourEventInfo : public G4VUserEventInformation {
public:
    YourEventInfo() = default;
    ~YourEventInfo() override = default;
    void Print() const override {}
    G4ThreeVector primaryPos;
    G4ThreeVector primaryDir;
};

#endif
