#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;

class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction();
    virtual ~TrackingAction();

    virtual void PostUserTrackingAction(const G4Track*);
};

#endif
