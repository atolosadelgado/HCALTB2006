// for building the detector
#include "G4GDMLParser.hh"
// run manager
#include "G4RunManagerFactory.hh"
// UI + vis
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"


#include "G4PhysListFactory.hh"
#include "YourDetectorConstructor.hh"
#include "YourActionInitialization.hh"

//________________________________________________________________________________
#include <iostream>
#include <string>

std::string get_basename(const std::string& filepath) {
    // find last slash
    size_t lastSlash = filepath.find_last_of("/\\");
    std::string filename = (lastSlash == std::string::npos) ? filepath : filepath.substr(lastSlash + 1);

    // find last dot
    size_t lastDot = filename.find_last_of('.');
    if (lastDot == std::string::npos) {
        // no extension
        return filename;
    }

    return filename.substr(0, lastDot);
}

//________________________________________________________________________________

void help(int argc, char** argv){
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << argv[0] << " <geometry.gdml> <physics list> <gun.mac> <ofilenmae>" << std::endl;
    std::cout << "\t  <physics option>: name of physics list to be used (e.g., FTFP_BERT_EMZ)" << std::endl;
}

int main(int argc, char** argv)
{
    help(argc, argv);

    auto geometry_filename = argv[1];
    G4String physics_list_name = G4String( argv[2] );
    bool vis_mode = false;

    G4String g4macro_filename;

    // if g4 macro file is provided
    if( argc >= 4 )
        g4macro_filename =G4String( argv[3] );
    else
        vis_mode = true;

    // create ofilename based on input parameters
    G4String ofilename = "meanEnergyResponse";
        ofilename += "_";
        ofilename += get_basename(geometry_filename);
        ofilename += "_";
        ofilename += physics_list_name;
        ofilename += ".root";
    if( argc >= 5 )
        ofilename = G4String( argv[4] );

    std::cout << "Configuration:\n";
    std::cout << "\tGDML: "<< geometry_filename << "\n";
    std::cout << "\tPL: "<< physics_list_name << "\n";
    std::cout << "\tMac: "<< g4macro_filename << "\n";
    std::cout << "\tofile: "<< ofilename<< "\n";


    // create run manager
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);

    // create detector from GDML file
    YourDetectorConstructor * user_detector_constructor = new YourDetectorConstructor(geometry_filename);
    runManager->SetUserInitialization(user_detector_constructor);

    // create Physics factory
    G4PhysListFactory pl_factory;
    auto physics_list = pl_factory.GetReferencePhysList( physics_list_name );
    if( ! physics_list ) throw std::runtime_error("No physics list named <"+ physics_list_name+"> found");
    runManager->SetUserInitialization(physics_list);

    // create user actions
    runManager->SetUserInitialization(new YourActionInitialization(ofilename));

    // initialize detector and physics
    runManager->Initialize();


    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    if(vis_mode)
    {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else
    {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + g4macro_filename);
    }


    //________________________________________________________________________________

    delete visManager;
    delete runManager;

    return 0;
}

