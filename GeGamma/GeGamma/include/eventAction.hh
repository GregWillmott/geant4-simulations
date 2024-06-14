// Set up our event action class for the .cc to pull from
#ifndef eventAction_h
#define eventAction_h 1

// G4 libraries needed
#include "G4UserEventAction.hh"
#include "globals.hh"

// Class needed to be referred to in the .cc and here, yet the definition is elsewhere
class OurRunAction;

class OurEventAction : public G4UserEventAction
{
    public:
        OurEventAction() = default; // Constructor
        ~OurEventAction() override = default; // Destructor

        void BeginOfEventAction(const G4Event* event) override; // Base G4 member that we can customise
        void EndOfEventAction(const G4Event* event) override; //                   "

        void AddEDep(G4double dE);
    private:
        // OurRunAction* fRunAction = nullptr;
        G4double EventEDep = 0.;
};

// Inline function, like Python delta functions. Perfect for one-liners
inline void OurEventAction::AddEDep(G4double dE){
    // dE: energy from a vertex inside the Ge crystal
    // EDep: total energy deposited from one particle lifetime = one pulse in the gamma spec
    EventEDep += dE;
}

#endif