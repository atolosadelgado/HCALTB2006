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
    // void   Reset_event_energy(){event_energy = 0;}
    double event_energy;
    int verbosity;
    // HCalSD_config cms_config;
    // LV attached to HCalSD (dumped from CMSSW)
    std::vector<std::string> sensitive_lv = { "HBScintillatorLayer0In1",
                                            "HBScintillatorLayer0In2",
                                            "HBScintillatorLayer1In1",
                                            "HBScintillatorLayer1In2",
                                            "HBScintillatorLayer2In1",
                                            "HBScintillatorLayer2In2",
                                            "HBScintillatorLayer3In1",
                                            "HBScintillatorLayer3In2",
                                            "HBScintillatorLayer4In1",
                                            "HBScintillatorLayer4In2",
                                            "HBScintillatorLayer5In1",
                                            "HBScintillatorLayer5In2",
                                            "HBScintillatorLayer6In1",
                                            "HBScintillatorLayer6In2",
                                            "HBScintillatorLayer7In1",
                                            "HBScintillatorLayer7In2",
                                            "HBScintillatorLayer8In1",
                                            "HBScintillatorLayer8In2",
                                            "HBScintillatorLayer9In1",
                                            "HBScintillatorLayer9In2",
                                            "HBScintillatorLayer10In1",
                                            "HBScintillatorLayer10In2",
                                            "HBScintillatorLayer11In1",
                                            "HBScintillatorLayer11In2",
                                            "HBScintillatorLayer12In1",
                                            "HBScintillatorLayer12In2",
                                            "HBScintillatorLayer13In1",
                                            "HBScintillatorLayer13In2",
                                            "HBScintillatorLayer14In1",
                                            "HBScintillatorLayer14In2",
                                            "HBScintillatorLayer15In1",
                                            "HBScintillatorLayer15In2",
                                            "HBScintillatorLayer16In1",
                                            "HBScintillatorLayer16In2",
                                            "HEScintillatorPart3Layer02Phi0",
                                            "HEScintillatorPart3Layer02Phi1",
                                            "HEScintillatorPart3Layer03Phi0",
                                            "HEScintillatorPart3Layer03Phi1",
                                            "HEScintillatorPart3Layer04Phi0",
                                            "HEScintillatorPart3Layer04Phi1",
                                            "HEScintillatorPart3Layer05Phi0",
                                            "HEScintillatorPart3Layer05Phi1",
                                            "HEScintillatorPart3Layer06Phi0",
                                            "HEScintillatorPart3Layer06Phi1",
                                            "HEScintillatorPart4Layer07Phi0",
                                            "HEScintillatorPart4Layer07Phi1",
                                            "HEScintillatorPart4Layer08Phi0",
                                            "HEScintillatorPart4Layer08Phi1",
                                            "HEScintillatorPart4Layer09Phi0",
                                            "HEScintillatorPart4Layer09Phi1",
                                            "HEScintillatorPart4Layer10Phi0",
                                            "HEScintillatorPart4Layer10Phi1",
                                            "HEScintillatorPart4Layer11Phi0",
                                            "HEScintillatorPart4Layer11Phi1",
                                            "HEScintillatorPart4Layer12Phi0",
                                            "HEScintillatorPart4Layer12Phi1",
                                            "HEScintillatorPart5Layer13Phi0",
                                            "HEScintillatorPart5Layer13Phi1",
                                            "HEScintillatorPart5Layer14Phi0",
                                            "HEScintillatorPart5Layer14Phi1",
                                            "HEScintillatorPart5Layer15Phi0",
                                            "HEScintillatorPart5Layer15Phi1",
                                            "HEScintillatorPart5Layer16Phi0",
                                            "HEScintillatorPart5Layer16Phi1",
                                            "HEScintillatorPart5Layer17Phi0",
                                            "HEScintillatorPart5Layer17Phi1",
                                            "HEScintillatorPart5Layer18Phi0",
                                            "HEScintillatorPart5Layer18Phi1",
                                            "HTSensLayer0",
                                            "HTSensLayer1",
                                            "HTSensLayer2",
                                            "HTSensLayer3"
                                          }; // end vector sensitive_lv
};

#endif
