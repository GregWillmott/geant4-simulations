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
    // Get the volume that the particle is in
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    G4double stepEnergy;
    G4ThreeVector stepPos;
    G4double mag;
    G4double prob;
    
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
    if(volume->GetName() == "GeClover"){
        // Get the energy deposited into the volume. This method should be protected
        // from double counting the energy loss.
        stepEnergy = step->GetTotalEnergyDeposit();

        // Get the vector of the particle's position
        stepPos = step->GetPreStepPoint()->GetPosition();
        mag = stepPos.mag();

        // Generate a random float between 0 and 1.
        prob = G4UniformRand();
    
        // Use mag to determine efficiency at this radius
        // G4cout << mag << " " << prob << G4endl;
        // 180 -> 185 mm: 0% eff
        // Do nothing

        // 185 -> 235 mm: 100% eff
        if ((mag > 185.*mm) && (mag < 235.*mm)){
            fEventAction->AddEDep(stepEnergy);
            // G4cout << "tally" << G4endl;
        }

        // 235 -> 240 mm: 90% eff
        else if ((mag > 235.*mm) && (mag < 240.*mm)){
            if (prob <= 0.9){
                fEventAction->AddEDep(stepEnergy);
                // G4cout << "tally" << G4endl;
            }
        }

        // 240 -> 245 mm: 90% eff
        else if ((mag > 240.*mm) && (mag < 245.*mm)){
            if (prob <= 0.8){
                fEventAction->AddEDep(stepEnergy);
                // G4cout << "tally" << G4endl;
            }
        }

        // 245 -> 250 mm: 60% eff
        else if ((mag > 245.*mm) && (mag < 250.*mm)){
            if (prob <= 0.6){
                fEventAction->AddEDep(stepEnergy);
                // G4cout << "tally" << G4endl;
            }
        }

        // 250 -> 255 mm: 30% eff
        else if ((mag > 250.*mm) && (mag < 255.*mm)){
            if (prob <= 0.3){
                fEventAction->AddEDep(stepEnergy);
                // G4cout << "tally" << G4endl;
            }
        }

        // 255 -> 260 mm: 10% eff
        else if ((mag > 255.*mm) && (mag < 260.*mm)){
            if (prob <= 0.1){
                fEventAction->AddEDep(stepEnergy);
                // G4cout << "tally" << G4endl;
            }
        }

        // 260 -> 270 mm: 0% eff
        // Do nothing


        // fEventAction->AddEDep(stepEnergy);
    }
}
