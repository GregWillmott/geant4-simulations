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
#include "G4SystemOfUnits.hh"

// Class constructor. Event action class object (pointer) is passed as argument to a pointer object inside this class
OurSteppingAction::OurSteppingAction(const OurDetectorConstruction* detectorCon, OurEventAction* eventAction)
:   fDetectorCon(detectorCon),
    fEventAction(eventAction)
{}

// No destructor as it's specified as default in the header

// Operations we need to be done per step (vertex)
void OurSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Get the volume that the particle is in now
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    
    // Check if the volume is Germanium or Bismuth Germanate
    if((volume->GetName() == "GeCrystal") || (volume->GetName() == "BGO")){
        // Get the energy deposited into the volume. This method should be protected
        // from double counting the energy loss.
        stepEnergy = step->GetTotalEnergyDeposit();
        // Step energy is +='d, volume determines the destination of this data
        fEventAction->AddEDep(stepEnergy, volume->GetName());
    }
}
