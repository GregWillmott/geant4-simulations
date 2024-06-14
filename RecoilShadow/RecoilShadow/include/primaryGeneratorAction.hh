// Set up our primary generator action class for the .cc to pull from
#ifndef primaryGeneratorAction_h
#define primaryGeneratorAction_h 1

// G4 libraries needed
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh" // Replace with General particle source later as it's better

// Classes needed to be referred to in the .cc and here, yet the definitions are elsewhere
class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

// Our P.G.A class inherits from the Geant class, at public level
class OurPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        OurPrimaryGeneratorAction(); // constructor
        ~OurPrimaryGeneratorAction() override; // destructor
        // Method from the base class
        void GeneratePrimaries(G4Event*) override;

        // Method to access particle gun via its pointer
        const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    private:
        G4ParticleGun* fParticleGun = nullptr; // pointer to a G4 gun class object. Starts null, can't be accessed directly outside class
        G4GeneralParticleSource* fGPS = nullptr; // pointer to a G4 general particle source (like the gun but accepts macros at runtime)
        G4bool ifGeneralSource;
};

#endif