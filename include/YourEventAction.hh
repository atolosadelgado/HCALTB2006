#ifndef YourEventAction_h
#define YourEventAction_h

#include "G4UserEventAction.hh"

class YourPrimaryGenerator;

class YourEventAction : public G4UserEventAction {
public:
  void EndOfEventAction(const G4Event*) override;
  void SetPrimaryGenerator(YourPrimaryGenerator * p){fPrimaryGenerator=p;}
private:
    YourPrimaryGenerator * fPrimaryGenerator;
};

#endif
