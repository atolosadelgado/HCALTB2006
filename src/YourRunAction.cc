
#include "YourRunAction.hh"
#include "YourEventAction.hh"
#include "YourHistogram.hh"

#include "G4Material.hh"

#include <iostream>

YourRunAction::YourRunAction(std::string ofilename):
          G4UserRunAction(),
          _ofilename(ofilename)
          {
              fHenergyResponse = new YourHistogram(100,0,1);
        }


YourRunAction::~YourRunAction() {}


void YourRunAction::BeginOfRunAction(const G4Run* ) {

}

void YourRunAction::EndOfRunAction(const G4Run* ){
    fHenergyResponse->fill(0.123);
    double mean = fHenergyResponse->mean();
    double meanError = fHenergyResponse->meanError();
    std::cout << "Energy response (mean, mean error): " << mean << "\t" << meanError << std::endl;

}

void YourRunAction::SetPrimaryGenerator(YourPrimaryGenerator* g)
{
    fPrimaryGenerator = g;
}

