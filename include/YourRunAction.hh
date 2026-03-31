#ifndef YOURRUNACTION_HH
#define YOURRUNACTION_HH

#include "G4UserRunAction.hh"

#include "YourParticleInfo.hh"

#include <string>
#include <vector>

class YourPrimaryGenerator;
class YourEventAction;
class YourTrackingAction;
class YourDetectorConstructor;
class YourInputArgs;

class YourRunAction : public G4UserRunAction {

  public:

    YourRunAction(const YourInputArgs * args, const YourDetectorConstructor * detector);
    virtual ~YourRunAction();

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run* run) override;

    void BeginOutputTree();
    void EndOutputTree();
    void InitializeSecondaryTrackHistogram();

    void SetEventAction(YourEventAction * evtAct){fEventAction = evtAct;}
    void SetTrackingAction(YourTrackingAction * trkAct){fTrackingAction = trkAct;}

    std::string fOutputFileName = {""};
    const YourInputArgs * fInputArgs = {nullptr};
    YourEventAction * fEventAction = {nullptr};
    YourTrackingAction * fTrackingAction = {nullptr};
    const YourDetectorConstructor * fDetector = {nullptr};
    YourParticleInfoMap particleInfoMap;

    // this vectors will be assigned to G4Analysis
    // at the end of the event, report centroid
    // and RMS for each one
    std::vector<double> fEnergyProfile;
    std::vector<double> fRadiusProfile;

    int verbosity = 0;
    void PrintGeant4Configuration();
};

#endif
