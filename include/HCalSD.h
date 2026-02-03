#ifndef HCalSD_h
#define HCalSD_h

#include <string>

#include "G4VSensitiveDetector.hh"
#include "HCalSD_config.h"

class HCalSD : public G4VSensitiveDetector {
public:
    HCalSD(std::string fname):G4VSensitiveDetector(fname){ std::cout << "New HCalSD named " << fname.c_str() << std::endl; }
    ~HCalSD() override = default;
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;
    double getEnergyDeposit(G4Step* aStep);
    void Initialize(G4HCofThisEvent * ) override;
    void EndOfEvent(G4HCofThisEvent * ) override;
    void clearHits();
    double Get_event_energy(){return event_energy;}
    double event_energy;
    int verbosity;
    HCalSD_config cms_config;
};

#endif
