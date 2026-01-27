#include "YourActionInitialization.hh"
#include "YourPrimaryGenerator.hh"
#include "YourInputArgs.hh"

// #include "G4MaterialScanner.hh"
#include "G4AnalysisManager.hh"

YourActionInitialization::YourActionInitialization(const YourInputArgs * args)
:   G4VUserActionInitialization(), fInputArgs(args){
  // ms = new G4MaterialScanner();
  G4AnalysisManager::Instance();

}


YourActionInitialization::~YourActionInitialization() {}

void YourActionInitialization::BuildForMaster() const {
}

void YourActionInitialization::Build() const {
  // Set UserPrimaryGeneratorAction
  YourPrimaryGenerator * gen = new YourPrimaryGenerator();
  SetUserAction(gen);
}
