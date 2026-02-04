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

    void Initialize(G4HCofThisEvent * ) override;
    void EndOfEvent(G4HCofThisEvent * ) override;
    void clearHits();
    double Get_event_energy(){return event_energy;}
    double event_energy = {0};
    int verbosity = {1};
    // hardcoded members of ECalSD + CaloSD for ECal, as in CMSSW
    ECalSD_config cms_config;

    // functions from ECalSD, as in CMSSW
    void getBaseNumber(const G4Step *);
    double getBirkL3(const G4Step *);
    // method getDepth calculates too many things, reimplemented as computeCrystalDepth
    // uint16_t getDepth(const G4Step *);
    double computeCrystalDepth(const G4Step* aStep);
    double getEnergyDeposit(const G4Step* aStep);
    uint32_t setDetUnitId(const G4Step *) ;
    double curve_LY(const G4LogicalVolume* lv, const G4Step * step);

    // Methods not migrated:
    // EcalBarrelNumberingScheme::getUnitID
    // EcalBaseNumber::addLevel
    // EcalBaseNumber::getCopyNumber

    // Methods from CaloSD, as in CMSSW,
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;

    virtual int getTrackID(const G4Track*);
    // function to deal with hits, not needed here
    // struct CaloG4Hit{};
    // bool checkHit(int k = 0);
    // void cleanHitCollection();
    // CaloG4Hit* createNewHit(const G4Step*, const G4Track*, int k);
    // bool hitExists(const G4Step*, int k);
    // bool saveHit(CaloG4Hit*, int k = 0);
    // ??
    // void update(const BeginOfRun*) override;
    // void update(const BeginOfEvent*) override;
    // void update(const BeginOfTrack* trk) override;
    // void update(const EndOfTrack* trk) override;
    // void update(const ::EndOfEvent*) override;










};
#endif
