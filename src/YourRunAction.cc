
#include "YourPrimaryGenerator.hh"
#include "YourRunAction.hh"
#include "YourEventAction.hh"
#include "YourSteppingAction.hh"

#include "G4Material.hh"
#include "G4RegionStore.hh"
#include <iostream>

#include "G4AnalysisManager.hh"

#include "YourInputArgs.hh"



YourRunAction::YourRunAction(std::string ofilename, const YourInputArgs * args):
          G4UserRunAction(),
          _ofilename(ofilename),
           fInputArgs(args)
          {
            // to make UI commands available
            auto analysisManager = G4AnalysisManager::Instance();
}


YourRunAction::~YourRunAction() {}

#include "G4RunManager.hh"
#include "G4VUserActionInitialization.hh"
void YourRunAction::BeginOfRunAction(const G4Run*)
{
    this->ConstructOutputTree();
    this->BeginOutputTree();
    auto fSteppingAction_const = static_cast<const YourSteppingAction*>( G4RunManager::GetRunManager()->GetUserSteppingAction() );
    auto fSteppingAction = const_cast<YourSteppingAction*>(fSteppingAction_const);
    fSteppingAction->FindRegionsAndMaterials();



}

void YourRunAction::EndOfRunAction(const G4Run* ){
    this->EndOutputTree();
    if (G4Threading::IsMasterThread()){
        fInputArgs->SaveToROOTfile( G4AnalysisManager::Instance()->GetFileName());
    }

}

const YourPrimaryGenerator * YourRunAction::GetPrimaryGenerator()
{
    return static_cast<const YourPrimaryGenerator*>(
        G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()
    );
}

void YourRunAction::FillEventEnergy(double ecal_energy, double hcal_energy)
{
    if(0 > ecal_energy || 0 > hcal_energy) throw std::runtime_error("YourRunAction::FillEventEnergy cannot save negative energy");

    auto fPrimaryGenerator = this->GetPrimaryGenerator();
    double ecal_energy_MeV = ecal_energy / CLHEP::MeV;
    double ecal_eventEnergyResponse = ecal_energy_MeV / fPrimaryGenerator->E0_MeV;

    double hcal_energy_MeV = hcal_energy / CLHEP::MeV;
    double hcal_eventEnergyResponse = hcal_energy_MeV / fPrimaryGenerator->E0_MeV;

    this->FillOutputTree(ecal_eventEnergyResponse, hcal_eventEnergyResponse);
}

void YourRunAction::ConstructOutputTree()
{
  auto analysisManager = G4AnalysisManager::Instance();
//   analysisManager->SetDefaultFileType("root"); // set in macrofile
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);  // important for MT

  analysisManager->CreateNtuple("tree", "tree for HCAL 2006 TB experiment");
  analysisManager->CreateNtupleDColumn("ECAL_eresponse");
  analysisManager->CreateNtupleDColumn("HCAL_eresponse");
  analysisManager->FinishNtuple();
}

void YourRunAction::BeginOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    std::string ofilename = analysisManager->GetFileName();
    const std::string phys = fInputArgs->physics_list;
    const std::string birk = std::to_string(fInputArgs->saturation);

    // check position of '.'
    std::size_t dotPos = ofilename.find_last_of('.');

    if (std::string::npos != dotPos ) {
        // if it has extension
        std::string basename  = ofilename.substr(0, dotPos);
        std::string extension = ofilename.substr(dotPos); // including '.'

        ofilename = basename + "_" + phys + "_birk" + birk + extension;
    } else {
        // if no extension
        ofilename = ofilename + "_" + phys + "_birk" + birk;
    }
    analysisManager->SetFileName(ofilename);
    analysisManager->OpenFile(); // name set in macrofile
}

void YourRunAction::EndOutputTree()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}

void YourRunAction::FillOutputTree(double ecal_eresponse, double hcal_eresponse)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, ecal_eresponse);
    analysisManager->FillNtupleDColumn(1, hcal_eresponse);
    analysisManager->AddNtupleRow();
    if(0 < verbosity)
        G4cout << "\tadding new row: " << ecal_eresponse << "\t" << hcal_eresponse << std::endl;
}

