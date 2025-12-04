#ifndef YOURRUNACTION_HH
#define YOURRUNACTION_HH

#include "G4UserRunAction.hh"

#include <string>

class YourRunAction : public G4UserRunAction {

  public:

    YourRunAction(std::string ofilename);
    virtual ~YourRunAction();

    void   BeginOfRunAction(const G4Run* run) override;
    void   EndOfRunAction(const G4Run* run) override;

    std::string _ofilename;
};

#endif
