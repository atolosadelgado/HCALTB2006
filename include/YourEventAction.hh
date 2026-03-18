#ifndef YOUREVENTACTION_HH
#define YOUREVENTACTION_HH

#include "G4UserEventAction.hh"

class YourEventAction : public G4UserEventAction {
public:

  YourEventAction();
  ~YourEventAction() override = default;

  void BeginOfEventAction(const G4Event* evt) override;
  void EndOfEventAction(const G4Event* evt) override;

};

#endif
