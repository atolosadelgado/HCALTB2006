#ifndef YOURCLUSTERCUT_HH
#define YOURCLUSTERCUT_HH

#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Step;

// class to encapsulate detector reconstruction effects
// such as crystal clustering
class YourClusterCut{
public:
    YourClusterCut(){}
    YourClusterCut(G4double radialCut):fRadialCut(radialCut){}
    ~YourClusterCut(){}

    bool IsInside(const G4Step* aStep) const;

    // utility for calculating the distance to the shower axis
    // it is static so it can be used by other classes too
    static double Calculate_hitpos_to_shower_axis_distance(const G4Step* aStep, G4ThreeVector & dir0,G4ThreeVector & pos0);

    G4double fRadialCut = {DBL_MAX};
};

#endif
