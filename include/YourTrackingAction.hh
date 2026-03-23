#ifndef YOURTRACKINGACTION_HH
#define YOURTRACKINGACTION_HH

#include "YourParticleInfo.hh"
#include <map>

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;

class YourTrackingAction : public G4UserTrackingAction
{
public:
    YourTrackingAction();
    virtual ~YourTrackingAction();

    virtual void PostUserYourTrackingAction(const G4Track * track);
    virtual void PreUserTrackingAction(const G4Track * track);

    void SetParticleInfoMap(YourParticleInfoMap & m){fParticleInfoMap = m;}
    YourParticleInfoMap fParticleInfoMap;
    const YourParticleInfo & GetParticleInfo(const G4Track * track) const;
};

#endif
