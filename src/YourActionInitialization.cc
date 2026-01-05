
#include "YourActionInitialization.hh"
#include "YourPrimaryGenerator.hh"
#include "YourRunAction.hh"
#include "YourEventAction.hh"
#include "YourSteppingAction.hh"


YourActionInitialization::YourActionInitialization(std::string ofilename)
:   G4VUserActionInitialization(), _ofilename(ofilename){ }


YourActionInitialization::~YourActionInitialization() {}

void YourActionInitialization::BuildForMaster() const {
  SetUserAction(new YourRunAction(_ofilename));
}

void YourActionInitialization::Build() const {
  // Set UserPrimaryGeneratorAction
    YourPrimaryGenerator * gen = new YourPrimaryGenerator();
  SetUserAction(gen);

  // Set UserRunAction
  // run action owns histograms to be written at the end
  YourRunAction* runAction = new YourRunAction(_ofilename);
  runAction->SetPrimaryGenerator(gen);
  SetUserAction(runAction);

  // Set UserEventAction
  // event action accumulates energy deposited and radius per Z-bin, per event
  // end of event, update main histograms owned by run action and reset event histograms
  YourEventAction* eventAction = new YourEventAction(runAction,gen);
  SetUserAction(eventAction);

  // Set UserSteppingAction
  // steping action updates histograms owned by event action, for energy deposited and shower width
  YourSteppingAction * steppingAction = new YourSteppingAction(eventAction);
  runAction->SetSteppingAction(steppingAction);
  SetUserAction( steppingAction );
}
