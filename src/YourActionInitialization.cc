#include "YourActionInitialization.hh"
#include "YourPrimaryGenerator.hh"
#include "YourInputArgs.hh"
#include "YourRunAction.hh"
#include "YourEventAction.hh"

// #include "G4MaterialScanner.hh"

// call manager here to load UI commands
#include "G4AnalysisManager.hh"

YourActionInitialization::YourActionInitialization(const YourInputArgs * args)
:   G4VUserActionInitialization(), fInputArgs(args){
  // ms = new G4MaterialScanner();

  // call manager here to load UI commands
  G4AnalysisManager::Instance();

}


YourActionInitialization::~YourActionInitialization() {}

void YourActionInitialization::BuildForMaster() const {
  YourRunAction * run = new YourRunAction(fInputArgs);
  SetUserAction(run);
}

void YourActionInitialization::Build() const {
  // Set UserPrimaryGeneratorAction
  YourPrimaryGenerator * gen = new YourPrimaryGenerator();
  YourEventAction * evt = new YourEventAction();
  YourRunAction * run = new YourRunAction(fInputArgs);
  evt->SetPrimaryGenerator(gen);

  SetUserAction(run);
  SetUserAction(evt);
  SetUserAction(gen);
}
