// Micron S3 DSSD Geant4 simulation
// Greg Willmott, Uni of Surrey
// gw00431@surrey.ac.uk

// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

// C++ help: https://cplusplus.com/doc/tutorial/

// Include Project headers
#include "../include/physics.hh"
#include "../include/detectorConstruction.hh"
#include "../include/actionInitialisation.hh"
#include "../include/primaryGeneratorAction.hh"

// Include Geant headers
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4Timer.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

// ------------------------------------------------------------------
int main(int argc, char** argv){
    // Look at input args, none = boot GUI, or one (a macro name) = run in batch (terminal only) mode
    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ){
        ui = new G4UIExecutive(argc, argv);}

    // G4Timer object monitors the CPU time spent
    G4Timer* programTimer = new G4Timer();
    // Start the timer
    programTimer->Start();

    // Use G4SteppingVerboseWithUnits
    G4int precision = 5;
    G4SteppingVerbose::UseBestUnit(precision);

    // Set up random engine
    CLHEP::RanluxEngine defaultEngine(1234567,4);
    G4Random::setTheEngine(&defaultEngine);
    // Random seed for fair results, fixed seed for reproducability
    int seed = time(NULL);
    // int seed = 01234;
    G4Random::setTheSeed(seed);

    // Multithreading now comes by default with the standard run manager. MTRunManager was depricated.
    G4RunManager * runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    #ifdef G4MULTITHREADED
        G4int nThreads = 8; // Change number of threads here, I can make nThreads a command line argument if needed.
        runManager->SetNumberOfThreads(nThreads);
    #endif

    // ----------------
    // Geometry
    // ----------------
    OurDetectorConstruction* detectorCon = new OurDetectorConstruction();
    runManager->SetUserInitialization(new OurDetectorConstruction());

    // ----------------
    // Physics List
    // ----------------
    OurPhysics* physicsList = new OurPhysics(0); // Expected args: verbosity,
    runManager->SetUserInitialization(physicsList); // Use the physics list made

    // ----------------------
    // Action initialisation
    // ----------------------
    runManager->SetUserInitialization(new OurActionInitialization(detectorCon));

    // ------------------
    // Visualisation
    // ------------------
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if (! ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String macroName = argv[1];
        UImanager->ApplyCommand(command+macroName);
    }
    else {
        // interactive mode. GUI configued by a macro
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Timer
    programTimer->Stop();
    G4cout << "Total runtime: " << programTimer->GetRealElapsed() << " s" << G4endl;

    // Tidy up old C style pointers. Run manager deletion covers physics list and geometry deletion
    delete runManager;
    delete visManager;
    delete programTimer;

    G4cout << "Program complete." << G4endl;

    return 0;
}