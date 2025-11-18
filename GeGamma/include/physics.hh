// Set up our physics class for the .cc and main.cc to pull from
#ifndef physics_h
#define physics_h 1

// G4 libraries needed
#include "globals.hh"
#include "G4VModularPhysicsList.hh"

// Our physics list class inherits from the Geant class, at public level
class OurPhysics : public G4VModularPhysicsList
{
    // Class members with different levels of accessibility based on what's needed.
    public:
        OurPhysics(int verbosity); // Constructor
        virtual ~OurPhysics(); // Destructor for deleting class object at the end
    private:
        // OurPhysics(OurPhysics &);
        // OurPhysics & operator=(const OurPhysics &right);
};
#endif