
#include "YourActionInitialization.hh"
#include "YourPrimaryGenerator.hh"
#include "YourRunAction.hh"
#include "YourEventAction.hh"
#include "YourTrackingAction.hh"
#include "YourSteppingAction.hh"
#include "YourInputArgs.hh"

#include "G4MaterialScanner.hh"

YourActionInitialization::YourActionInitialization(const YourInputArgs * args, const YourDetectorConstructor * detector)
:   G4VUserActionInitialization(),fInputArgs(args), fDetector(detector){
  // ms = new G4MaterialScanner();
}


YourActionInitialization::~YourActionInitialization() {}

void YourActionInitialization::BuildForMaster() const {
  SetUserAction(new YourRunAction(fInputArgs, fDetector));
}

void YourActionInitialization::Build() const {
  // Set UserPrimaryGeneratorAction
    YourPrimaryGenerator * gen = new YourPrimaryGenerator();

  // Set UserRunAction
  // run action owns histograms to be written at the end
  YourRunAction* runAction = new YourRunAction(fInputArgs, fDetector);

  // Set UserEventAction
  // event action accumulates energy deposited and radius per Z-bin, per event
  // end of event, update main histograms owned by run action and reset event histograms
  YourEventAction* eventAction = new YourEventAction();

  // Set UserTracking action
  // tracking action stores information of G4Analysis to score initial and final energy
  // and life time of particle in the simulation
  YourTrackingAction * trackingAction = new YourTrackingAction();

  // Set User Stepping Action
  // stepping action calls EventAction methods to accumulate global quantities during the event
  YourSteppingAction * stepAction = new YourSteppingAction(eventAction);

  // RunAction open the G4Analysis, and creates NTuple
  // it must notify EventAction which is the ID for each branch
  // runAction->SetEventAction(eventAction);
  runAction->SetTrackingAction(trackingAction);
  // RunAction configures the event action at the begining of the run
  runAction->SetEventAction(eventAction);

  SetUserAction(gen);
  SetUserAction(runAction);
  SetUserAction(eventAction);
  SetUserAction(trackingAction);
  SetUserAction(stepAction);
}
