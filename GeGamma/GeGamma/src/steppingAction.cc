// Grab our corresponding header for this class
#include "../include/steppingAction.hh"

// More of the project files are needed here
#include "../include/eventAction.hh"
#include "../include/detectorConstruction.hh"

// G4 libraries needed
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

// Class constructor. Event action class object (pointer) is passed as argument to a pointer object inside this class
OurSteppingAction::OurSteppingAction(const OurDetectorConstruction* detectorCon, OurEventAction* eventAction)
:   fDetectorCon(detectorCon),
    fEventAction(eventAction)
{}

// No destructor as it's specified as default in the header

// Operations we need to be done per step (vertex)
void OurSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Get the volume that the particle is in
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

    // Debug 04/10
    // G4cout << volume->GetName() << G4endl;
    // G4cout << fDetectorCon->GetGermaniumPV()->GetName() << G4endl; // causes seg fault
    // Example B4 compared physical volumes directly but this doesn't work here
    // Comparing the names isn't possible as line 34 causes a seg fault so temporary solution
    // is to hard code the expected answer to gate by material.

    // Weight is only affected by importance biasing, the intensities in the source definition are not found here
    // G4double weight = step->GetTrack()->GetWeight();
    // G4cout << weight << G4endl;

    // Check if the volume is Germanium.
    if(volume->GetName() == "Ge-cyl"){
        // Get the energy deposited into the volume. This method should be protected
        // from double counting the energy loss.
        G4double StepEnergy = step->GetTotalEnergyDeposit();

        // Call inline function to add energy deposited (definition in eventAction.hh)
        fEventAction->AddEDep(StepEnergy);
    }
}
