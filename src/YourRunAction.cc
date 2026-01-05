
#include "YourPrimaryGenerator.hh"
#include "YourRunAction.hh"
#include "YourEventAction.hh"
#include "YourSteppingAction.hh"
#include "HistoEnergyResponse.hh"

#include "G4Material.hh"
#include "G4RegionStore.hh"
#include <iostream>

#include "TFile.h"

YourRunAction::YourRunAction(std::string ofilename):
          G4UserRunAction(),
          _ofilename(ofilename)
          {
              fHenergyResponse = new HistoEnergyResponse("HCAL2006TB", 100,0.,1.);
        }


YourRunAction::~YourRunAction() {}


void YourRunAction::BeginOfRunAction(const G4Run* ) {

        G4Region * ecal_region =  G4RegionStore::GetInstance()->GetRegion("EcalRegion");
        if(nullptr == ecal_region) throw std::runtime_error("No ECAL region found in G4RegionStore");
        else                       fSteppingAction->SetEcalRegion(ecal_region);

        G4Region * hcal_region =  G4RegionStore::GetInstance()->GetRegion("HcalRegion");
        if(nullptr == hcal_region) throw std::runtime_error("No HCAL region found in G4RegionStore");
        else                       fSteppingAction->SetHcalRegion(hcal_region);
}

void YourRunAction::EndOfRunAction(const G4Run* ){
    double mean = fHenergyResponse->ecal->mean();
    double meanError = fHenergyResponse->ecal->meanError();
    std::cout << "ECAL Energy response (mean, mean error): " << mean << "\t" << meanError << std::endl;
    mean = fHenergyResponse->hcal->mean();
    meanError = fHenergyResponse->hcal->meanError();
    std::cout << "HCAL Energy response (mean, mean error): " << mean << "\t" << meanError << std::endl;

    TFile * ofile = TFile::Open( "test.root" , "recreate");
    fHenergyResponse->write(ofile);
    ofile->Close();

}

void YourRunAction::SetPrimaryGenerator(YourPrimaryGenerator* g)
{
    fPrimaryGenerator = g;
}

void YourRunAction::SetSteppingAction ( YourSteppingAction* s )
{
    fSteppingAction = s;
}


void YourRunAction::FillEventEnergyECAL(double energy)
{
    double energy_MeV = energy / CLHEP::MeV;
    double eventEnergyResponse = energy_MeV / fPrimaryGenerator->E0_MeV;
    fHenergyResponse->ecal->fill(eventEnergyResponse);
}


void YourRunAction::FillEventEnergyHCAL(double energy)
{
    double energy_MeV = energy / CLHEP::MeV;
    double eventEnergyResponse = energy_MeV / fPrimaryGenerator->E0_MeV;
    fHenergyResponse->hcal->fill(eventEnergyResponse);
}

