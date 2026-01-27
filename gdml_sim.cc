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

#include "SimpleCaloSD.h"

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

#include "YourInputArgsParser.hh"

int main(int argc, char** argv)
{
    YourInputArgParser parser(argc, argv);

    if (!parser.parse())
    {
        YourInputArgParser::help(argv[0]);
        return -1;
    }

    const YourInputArgs & iargs = parser.args();

    std::cout << iargs;

    // create run manager
    G4RunManager* runManager;
    if(1 < iargs.nthreads){
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
        runManager->SetNumberOfThreads( iargs.nthreads );
    }
    else
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);

    // create detector from GDML file
    YourDetectorConstructor * user_detector_constructor = new YourDetectorConstructor(iargs.geometry);
    if(iargs.airECAL){
        user_detector_constructor->ConfigureECALAsAir();
    }
    if(iargs.visSensitiveOnly)
        user_detector_constructor->ConfigureVisSensitiveOnly();

    runManager->SetUserInitialization(user_detector_constructor);

    // create Physics factory
    G4PhysListFactory pl_factory;
    auto physics_list = pl_factory.GetReferencePhysList( iargs.physics_list );
    if( ! physics_list ) throw std::runtime_error("No physics list named <"+ iargs.physics_list+"> found");
    runManager->SetUserInitialization(physics_list);

    // create user actions
    runManager->SetUserInitialization(new YourActionInitialization(&iargs));

    // initialize detector and physics
    runManager->Initialize();


    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand(
        "/control/alias PARTICLE_ENERGY_GEV " +
        std::to_string(iargs.particle_energy_GeV)
    );
    UImanager->ApplyCommand("/control/alias PARTICLE_NAME " + iargs.particle_name);
    UImanager->ApplyCommand(
        "/control/alias NEVENTS " +
        std::to_string(iargs.nevents)
    );
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    if( true == iargs.vis_mode )
    {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else
    {

        UImanager->ApplyCommand("/control/execute " + iargs.macro);
    }


    //________________________________________________________________________________

    delete visManager;
    delete runManager;

    return 0;
}

