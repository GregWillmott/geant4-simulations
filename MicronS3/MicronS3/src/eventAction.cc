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
    eventEDep = 0.;
    PbEDep = 0.;
    hitPixelCopyNum = 999; // 0 is taken
    X = 0.;
    Y = 0.;
    sectNum = 999; // default needs to be out of range
    ringNum = 999;
    evMult = 0;
    name = "";
    hitPxls = {};
}

void OurEventAction::EndOfEventAction(const G4Event*)
{
    // Tallying per particle history is put into histograms here

    // Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Use results of tally funtion:
    if(eventEDep != 0.){ // stop millions of zeros being included. If you want to estimate efficiency comment the if out
        // Energy spectrum
        analysisManager->FillNtupleDColumn(0, eventEDep); // Args: nTuple index, number to put in
        analysisManager->FillNtupleDColumn(1, X);
        analysisManager->FillNtupleDColumn(2, Y);

        // Hit pattern
        G4double r = sqrt(pow(posVec.x(),2) + pow(posVec.y(),2));
        G4double phi = posVec.phi() + CLHEP::pi;

        ringNum = hitPixelCopyNum / 32;
        sectNum = hitPixelCopyNum % 32;
        // G4cout << sectNum << G4endl;

        analysisManager->FillH2(0, phi, r); // args: hist ID, x entry, y entry
        analysisManager->FillH2(1, sectNum, ringNum+11.5); // args: hist ID, x entry, y entry


        analysisManager->FillNtupleDColumn(3, phi);
        analysisManager->FillNtupleDColumn(4, r);

        // Multiplicities
        // basic mult in event
        analysisManager->FillNtupleDColumn(5, evMult);
        // pixel mult
        // G4cout << "test" << nUniqueFromVec(hitPxls) << G4endl;
        analysisManager->FillNtupleDColumn(6, nUniqueFromVec(hitPxls));

        // analysisManager->AddNtupleRow();
    }
    // if(PbEDep != 0.){
        // Beam energy spectrum
        analysisManager->FillNtupleDColumn(7,PbEDep);
        analysisManager->AddNtupleRow();
    // }
}

void OurEventAction::MyTally(G4double dE, G4int pixelID, G4ThreeVector pos, G4String partName){
    // dE: energy from a vertex inside the Si pixel
    // pixelID: label of physical volume hit by particle
    // EDep: total energy deposited from one particle lifetime = one pulse in the energy spectrum
    // hitPixel: label of pixel to get X, Y position from for the ROOT file
    // If(in silicon) has already been checked at this point

    eventEDep += dE;
    hitPixelCopyNum = pixelID;
    posVec = pos;
    X = pos.x();
    Y = pos.y();
    name = partName;
    evMult += 1;
    
    hitPxls.push_back(hitPixelCopyNum);
    // if(name != "") std::cout << name << std::endl;
}

void OurEventAction::MyTally2(G4double dE){
    PbEDep += dE;
}

int OurEventAction::nUniqueFromVec(std::vector<int> vec){
    int counter;
    int nUnique = 0;
    int threshold;
    for(int i=0; i < vec.size(); i++){
        // Setup
        counter = 0;
        threshold = vec.size() - (i+1);
        // Count how many different entries there are to the ith
        // Don't back count so the last entry of a given value is included
        for(int j=i; j < vec.size(); j++){
            if(vec[i] != vec[j]) counter++;
        }
        // Are there enough counts so that i is unique?
        if(counter >= threshold){
            nUnique++;
        }
    }
    return nUnique;
}