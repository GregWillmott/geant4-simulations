// Set up the action initialisation (particle source) class for the .cc
#ifndef actionInitialisation_h
#define actionInitialisation_h 1

// G4 libraries needed
#include "G4VUserActionInitialization.hh"

// Class needed to be referred to in the .cc and here, yet the definitions are elsewhere
class OurDetectorConstruction;

// Our AcInit class inherits from the Geant class, at public level
class OurActionInitialization : public G4VUserActionInitialization
{
    public:
        OurActionInitialization(OurDetectorConstruction*); // Constructor
        ~OurActionInitialization() override = default; // Destructor
            
        void BuildForMaster() const override; // Master = master thread. Fewer requirements than a worker thread because of no transport
        void Build() const override; // Function from G4 that's customised
    private:
        OurDetectorConstruction* fDetectorCon = nullptr;
};
#endif