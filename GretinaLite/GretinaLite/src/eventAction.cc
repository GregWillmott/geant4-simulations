// Grab our corresponding header for this class
#include "../include/eventAction.hh"

// More of the project files are needed here
#include "../include/runAction.hh"

// G4 libraries needed
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh" // Old G4 versions treated CSV, HDF5, XML, and Root AM's separately. Now all file output formats are here.

// OurEventAction::OurEventAction(OurRunAction* runAction)
// : fRunAction(runAction)
// {} // Constructor, initialises with the run action class object's pointer assigned to pointer object (f prefix is convention for obj of this purpose) fRunAction

// No destructor as its specified as default in the header

// Base G4 member functions, but with our options put in
void OurEventAction::BeginOfEventAction(const G4Event*)
{
    // Anything that has to be reset for each new particle history goes here
    EventEDep = 0.;

    // G4cout << "Begin of Event" << G4endl;
}

void OurEventAction::EndOfEventAction(const G4Event*)
{
    // Tallying per particle history is put into histograms here

    // Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Fill root histogram, nTuple
    // G4cout << EventEDep << G4endl;

    // Reject too energetic events
    // if (EventEDep > 3.*MeV){
    //     EventEDep = 0.;
    // }

    // analysisManager->FillNtupleDColumn(0, EventEDep);
    analysisManager->FillH1(0, EventEDep); // Args: histo index, number to put into histo
    // analysisManager->AddNtupleRow();
}