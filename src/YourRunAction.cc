
#include "YourPrimaryGenerator.hh"
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
    double mean = fHenergyResponse->mean();
    double meanError = fHenergyResponse->meanError();
    std::cout << "Energy response (mean, mean error): " << mean << "\t" << meanError << std::endl;

}

void YourRunAction::SetPrimaryGenerator(YourPrimaryGenerator* g)
{
    fPrimaryGenerator = g;
}

void YourRunAction::FillEventEnergy(double energy)
{
    double energy_MeV = energy / CLHEP::MeV;
    double eventEnergyResponse = energy_MeV / fPrimaryGenerator->E0_MeV;
    fHenergyResponse->fill(eventEnergyResponse);
}

