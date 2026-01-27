#ifndef SimpleCaloSD_h
#define SimpleCaloSD_h

#include <string>
#include <iostream>

#include "G4VSensitiveDetector.hh"

/* class SimpleCaloSD
 * reimplements CMSSW classes
 * EcalSD -> CaloSD -> SensitiveCaloDetector -> SensitiveDetector
 * in one single class
 * Ignoring at the moment:
 *      EnergyCorrected, getTrackID, processSecondHit
 *      initMap, getRadiationLength, getLayerIDForTimeSim
 *      Birks / slope / weighting / ageing
 *      maps per cell (hitMap, CaloHitID)
 *      fillHits (EDM), drawAll, printAll
 *
 */
class SimpleCaloSD : public G4VSensitiveDetector {
public:
    SimpleCaloSD(std::string fname):G4VSensitiveDetector(fname){ std::cout << "New EcalSD named " << fname.c_str() << std::endl; }
    ~SimpleCaloSD() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;
    double getEnergyDeposit(G4Step* aStep);
    void Initialize(G4HCofThisEvent * ) override;
    void EndOfEvent(G4HCofThisEvent * ) override;
    void clearHits();
private:
    double event_energy;
    int verbosity = 1;
};

#endif
