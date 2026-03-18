#ifndef YOURRUNACTION_HH
#define YOURRUNACTION_HH

#include "G4UserRunAction.hh"

#include <string>

class YourPrimaryGenerator;
class YourEventAction;
class YourInputArgs;

class YourRunAction : public G4UserRunAction {

  public:

    YourRunAction(const std::string & ofilename, const YourInputArgs * args);
    virtual ~YourRunAction();

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run* run) override;

    void BeginOutputTree();
    void EndOutputTree();

    void SetEventAction(YourEventAction * evtAct){fEventAction = evtAct;}

    std::string fOutputFileName = {""};
    const YourInputArgs * fInputArgs;
    YourEventAction * fEventAction;

    int verbosity = 0;
};

#endif
