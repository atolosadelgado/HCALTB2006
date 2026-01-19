#include "YourEventAction.hh"
#include "YourRunAction.hh"
#include "YourPrimaryGenerator.hh"

#include "G4EventManager.hh"


YourEventAction::YourEventAction(YourRunAction * myRunAction, YourPrimaryGenerator * gen)
  : G4UserEventAction(),
  fRunAction(myRunAction), fPrimaryGenerator(gen), fTimeMax(50*CLHEP::ns) {

  }


YourEventAction::~YourEventAction() {

}

void YourEventAction::BeginOfEventAction(const G4Event* anEvent) {
  visible_energy_ecal = 0;
  visible_energy_hcal = 0;
  time_first_hit = 0;
  time_last_hit = 0;

  // superseeded by UI command /run/printProgress 50
  // if( anEvent->GetEventID() % 10 == 0 )
  //   std::cout << "Event\t" << anEvent->GetEventID() << std::endl;

}


void YourEventAction::EndOfEventAction(const G4Event* /*evt*/) {
  fRunAction->FillEventEnergy(visible_energy_ecal,visible_energy_hcal);
  if(0 < fRunAction->verbosity)
    G4cout << "\tTime duration of event in calorimeters : "
              << (time_last_hit - time_first_hit) / CLHEP::ns
              << " ns" << std::endl;
}

void YourEventAction::RegisterTime(double t)
{
  if(0 == time_first_hit)
    time_first_hit = t;
  time_last_hit = std::max(time_last_hit,t);
}

bool YourEventAction::IsTimeWithinEventTimeWindow(double t)
{
  // initialize if needed
  if(0 == time_first_hit)
    time_first_hit = t;

  // time is within the event window if
  // time since event started in the calorimeter (t-time_first_hit)
  // is smaller than the limit fTimeMax
  bool IsTimeWithinEventTimeWindow_val = t-time_first_hit<fTimeMax;
  if(IsTimeWithinEventTimeWindow_val)
    time_last_hit = std::max(time_last_hit,t);
  return IsTimeWithinEventTimeWindow_val;
}

