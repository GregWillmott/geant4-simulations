// Set up our stepping action class for the .cc to pull from
#ifndef steppingAction_h
#define steppingAction_h 1

// G4 libraries needed
#include "G4UserSteppingAction.hh"
#include "globals.hh"

// Classes needed to be referred to in the .cc and here, yet the definitions are elsewhere
class OurEventAction;
class OurDetectorConstruction;

// Our stepping action class inherits from the Geant class, at public level
class OurSteppingAction : public G4UserSteppingAction
{
    public:
        // Constructor: detector con, event action class objects (pointers) set up
        OurSteppingAction(const OurDetectorConstruction* detectorCon, OurEventAction* eventAction); 
        ~OurSteppingAction() override = default;

        // Method from the base class
        void UserSteppingAction(const G4Step*) override;

        G4double stepEnergy;
        // G4ThreeVector stepPos;
        // G4double mag;
        // G4double prob;

    private:
        OurEventAction* fEventAction = nullptr;
        const OurDetectorConstruction* fDetectorCon = nullptr;
};

#endif