// Grab our corresponding header for this class
#include "../include/runAction.hh"

// More of the project files are needed here
#include "../include/primaryGeneratorAction.hh"
#include "../include/detectorConstruction.hh"

// G4 libraries needed
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh" // Old G4 versions treated CSV, HDF5, XML, and Root A.M.'s separately. Now all file output formats are here.

// C++ libraries
#include <iostream>
#include <string>
#include <ctime>
#include <math.h>

// Class constructor
OurRunAction::OurRunAction()
{
    // Printing event number / 200000 events
    G4RunManager::GetRunManager()->SetPrintProgress(200000);

    // Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Create directories
    // analysisManager->SetHistoDirectoryName("histograms");
    // analysisManager->SetNtupleDirectoryName("ntuple");
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true); // This scoops up the data from all threads into one output in a threadsafe manner
    // Note: merging ntuples is available only with Root output

    // Create histos and nTuple

    // First histo, index 0 automatically
    // analysisManager->CreateH1("EDep Hist", "Energy deposited in S3 DSSD", bins, EMin, EMax); // name, title, nbins, xmin, xmax
    // Hitpat
    analysisManager->CreateH2("hitPatHist", "Hit pattern in DSSD. x=Z, y=Y", 72, -36., 36., 72, -36., 36.);
    analysisManager->CreateH2("hitPatHistID", "Hit pattern in DSSD by IDs. x=sectID, y=ringID", 72, -36., 36., 72, -36., 36.);

    // First nTuple
    analysisManager->CreateNtuple("RecoilShadowSim", "N(electrons), EDep spec for Geant");
    // Column idx 0
    analysisManager->CreateNtupleDColumn("Nelec");
    // Column idx 1
    analysisManager->CreateNtupleDColumn("EDep");
    analysisManager->FinishNtuple();
}

// No destructor as it's specified as default in the header.

// G4 members for run action. Begin is for stuff that needs to be done pre radiation transport.
void OurRunAction::BeginOfRunAction(const G4Run*)
{
    G4cout << "Initialisation complete, run beginning." << G4endl; // Marks construction of geometry etc and the start of transport

    // Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%d-%m-%Y-%H-%M-%S", timeinfo);
    std::string timeStr(buffer.data());

    // G4String filename = "RSM-" + G4String(timeStr) + ".root";
    G4String filename = "test.root";

    // For different formats change .root to .csv, .hdf5, .xml
    analysisManager->OpenFile(filename);
}

void OurRunAction::EndOfRunAction(const G4Run* run)
{
    // Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}