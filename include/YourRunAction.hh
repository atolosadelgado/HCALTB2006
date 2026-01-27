#ifndef YourRunAction_h
#define YourRunAction_h

#include <string>

#include "G4UserRunAction.hh"

class YourPrimaryGenerator;
class YourInputArgs;

class YourRunAction : public G4UserRunAction {

  public:

    YourRunAction(const YourInputArgs * args);
    virtual ~YourRunAction();

    void   BeginOfRunAction(const G4Run* run) override;
    void   EndOfRunAction(const G4Run* run) override;

    void ConstructOutputTree();
    void BeginOutputTree();
    void EndOutputTree();

    void SetPrimaryGenerator(YourPrimaryGenerator * p){fPrimaryGenerator = p;}

    std::string _ofilename;
    const YourInputArgs * fInputArgs;
    YourPrimaryGenerator * fPrimaryGenerator;

private:
    int verbosity = 0;
};

#endif
