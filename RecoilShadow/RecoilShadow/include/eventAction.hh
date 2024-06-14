// Set up our event action class for the .cc to pull from
#ifndef eventAction_h
#define eventAction_h 1

// G4 libraries needed
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

// Base C++
#include <map>
#include <cmath>

// Class needed to be referred to in the .cc and here, yet the definition is elsewhere
class OurRunAction;

class OurEventAction : public G4UserEventAction
{
    public:
        OurEventAction() = default; // Constructor
        ~OurEventAction() override = default; // Destructor

        void BeginOfEventAction(const G4Event* event) override; // Base G4 member that we can customise
        void EndOfEventAction(const G4Event* event) override; //                   "

        void MyTally(G4double, G4int, G4ThreeVector, G4String); // Function to pass EDep, pixel ID, position, and particle name from step to event.

        int nUniqueFromVec(std::vector<int>);
    private:
        // OurRunAction* fRunAction = nullptr;
        G4double eventEDep = 0.;
        G4ThreeVector posVec;
        G4int hitPixelCopyNum = 999;
        G4double X = 0.;
        G4double Y = 0.;
        G4String name = "";
        G4int nElec;
        G4int xIDNum, yIDNum, evMult, pxlMult;
        bool ifFirst;
        std::vector<int> hitPxls;
        std::vector<int> testVec = {1,1,2,1,3};
        // G4double vecXYs [800][2];

};


#endif