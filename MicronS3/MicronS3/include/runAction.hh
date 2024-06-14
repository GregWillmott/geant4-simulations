// Set up the run action class for the .cc to pull from
#ifndef runAction_h
#define runAction_h 1

// G4 libraries needed
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

// Class needed to be referred to in the .cc and here, yet the definitions are elsewhere
class G4Run;

// Our R.A class inherits from the Geant class, at public level
// Hierarchy: Top level - Run (the sim) which contains many Events
//            Mid level - Event (particle histories)
//            Base level - Step (verticies), many for each Event
class OurRunAction : public G4UserRunAction
{
    public:
        OurRunAction(); // Constructor
        ~OurRunAction() override = default; // Destructor

        void BeginOfRunAction(const G4Run*) override; // Base G4 member which we can add to
        void   EndOfRunAction(const G4Run*) override; //                 "
};

#endif