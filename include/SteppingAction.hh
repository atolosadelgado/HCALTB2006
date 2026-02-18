
#ifndef SteppingAction_hh
#define SteppingAction_hh

#include "G4UserSteppingAction.hh"
class SteppingAction : public G4UserSteppingAction{
public:
    SteppingAction() = default;
    void UserSteppingAction(const G4Step* aStep);
    ~SteppingAction() = default;
};

#endif
