#ifndef YourEventAction_h
#define YourEventAction_h

#include "G4UserEventAction.hh"

class YourEventAction : public G4UserEventAction {
public:
  void EndOfEventAction(const G4Event*) override;
};

#endif
