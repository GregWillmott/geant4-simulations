// Grab our corresponding header for this class
#include "../include/eventAction.hh"

// More of the project files are needed here
#include "../include/runAction.hh"

// G4 libraries needed
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh" // Old G4 versions treated CSV, HDF5, XML, and Root AM's separately. Now all file output formats are here.
#include "G4SystemOfUnits.hh"

// OurEventAction::OurEventAction(OurRunAction* runAction)
// : fRunAction(runAction)
// {} // Constructor, initialises with the run action class object's pointer assigned to pointer object (f prefix is convention for obj of this purpose) fRunAction

// No destructor as its specified as default in the header

// Base G4 member functions, but with our options put in
void OurEventAction::BeginOfEventAction(const G4Event*)
{
    // Anything that has to be reset for each new particle history goes here
    GeEventEDep = 0.;
    BGOEventEDep = 0.;
    if_Ge_hit = false;
    if_BGO_hit = false;
}

void OurEventAction::EndOfEventAction(const G4Event*)
{
    // Tallying per particle history is put into histograms here

    // Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Reject too energetic events
    // if (EventEDep > 3.*MeV){
    //     EventEDep = 0.;
    // }

    // Use results of tally funtion:
    // (1) Ge Full E Peak
    if((if_Ge_hit == true) && (if_BGO_hit == false)){
        analysisManager->FillH1(0, GeEventEDep); // Args: histo index, number to put into histo
    }
    // (2) Ge and BGO hit
    else if((if_Ge_hit == true) && (if_BGO_hit == true)){
        analysisManager->FillH1(0, GeEventEDep);
        analysisManager->FillH1(1, GeEventEDep); // Same count goes in so subtraction can be performed in ROOT
    }
    // (3) BGO only: No action, but manually tally zero to preserve root-entries = n_events
    else if((if_Ge_hit == false) && (if_BGO_hit == true)){
        analysisManager->FillH1(0, 0.);
    }
    // (4) Miss
    else if((if_Ge_hit == false) && (if_BGO_hit == false)){
        analysisManager->FillH1(0, GeEventEDep); // EDep will be 0 anyway
    }
}

void OurEventAction::AddEDep(G4double dE, G4String hitVol){
    // dE: energy from a vertex inside the Ge crystal
    // vol: volume this interaction happened in (BGO or GeCrystal)
    // EDep: total energy deposited from one particle lifetime = one pulse in the gamma spec

    // POSSIBLE OUTCOMES
    // (1) Ge true, BGO false: count in main spec
    // (2) Both true: compton suppression -> add to main and background specs
    // (3) BGO true: not registered, count 0. keV
    // (4) Both false: put a 0 count in main spec for efficiency estimates using bin 0

    // Once flipped to true, the boolean will only return false at the start of the next event
    // Steps auto happen at boundaries so a 100 keV threshold ensures an interaction actually happened
    // Ge and BGO energy deposited are kept in separate doubles.

    if(hitVol == "GeCrystal"){
        GeEventEDep += dE;
        if(dE > 10.*keV){
            // If interaction happened flip the boolean
            if_Ge_hit = true;
        }
    }
    if(hitVol == "BGO"){
        BGOEventEDep += dE;
        if(dE > 10.*keV){
            // If interaction happened flip the boolean
            if_BGO_hit = true;
        }
    }
}