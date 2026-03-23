#ifndef YOURRUNACTION_HH
#define YOURRUNACTION_HH

#include "G4UserRunAction.hh"

#include <string>

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

    // void SetEventAction(YourEventAction * evtAct){fEventAction = evtAct;}
    void SetTrackingAction(YourTrackingAction * trkAct){fTrackingAction = trkAct;}

    std::string fOutputFileName = {""};
    const YourInputArgs * fInputArgs;
    // YourEventAction * fEventAction;
    YourTrackingAction * fTrackingAction;
    const YourDetectorConstructor * fDetector;

    int verbosity = 0;
};

#endif
