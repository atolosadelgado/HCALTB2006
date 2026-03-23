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

    virtual void PostUserTrackingAction(const G4Track * track) override;
    virtual void PreUserTrackingAction(const G4Track * track) override;

    void SetParticleInfoMap(YourParticleInfoMap & m){fParticleInfoMap = m;}
    YourParticleInfoMap fParticleInfoMap;
    const YourParticleInfo & GetParticleInfo(const G4Track * track) const;
};

#endif
