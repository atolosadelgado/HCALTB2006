#ifndef ECalSD_h
#define ECalSD_h

#include "ECalSD_config.h"

#include <string>

#include "G4VSensitiveDetector.hh"

/* class ECalSD
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
class ECalSD : public G4VSensitiveDetector {
public:
    ECalSD(std::string fname):G4VSensitiveDetector(fname){ std::cout << "New EcalSD named " << fname.c_str() << std::endl; }
    ~ECalSD() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;
    double getEnergyDeposit(G4Step* aStep);
    void Initialize(G4HCofThisEvent * ) override;
    void EndOfEvent(G4HCofThisEvent * ) override;
    void clearHits();
    double Get_event_energy(){return event_energy;}
    double event_energy = {0};
    int verbosity = {0};
    ECalSD_config cms_config;
};
#endif
