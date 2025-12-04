
#include "YourRunAction.hh"

#include "G4EventManager.hh"
#include "YourEventAction.hh"

#include "TH1D.h"

#include "G4Material.hh"

YourRunAction::YourRunAction(std::string ofilename):
          G4UserRunAction(),
          _ofilename(ofilename)
          {}


YourRunAction::~YourRunAction() {}


void YourRunAction::BeginOfRunAction(const G4Run* ) {

}

void YourRunAction::EndOfRunAction(const G4Run* ){

}
