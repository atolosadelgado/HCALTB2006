#include "TrackingAction.hh"

#include "G4Track.hh"
#include "G4Neutron.hh"
#include "G4EventManager.hh"

#include "YourEventAction.hh"

TrackingAction::TrackingAction()
: G4UserTrackingAction()
{}

TrackingAction::~TrackingAction()
{}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    // if(aTrack->GetDefinition() == G4Neutron::Neutron())
    // {
    //     YourEventAction* evt =
    //         dynamic_cast<YourEventAction*>(
    //             G4EventManager::GetEventManager()->GetUserEventAction()
    //         );
    //     if(evt)
    //     {
    //         evt->fNeutronEfinal.push_back(aTrack->GetKineticEnergy());
    //         evt->fNeutronTfinal.push_back(aTrack->GetLocalTime());
    //     }
    // }
}
