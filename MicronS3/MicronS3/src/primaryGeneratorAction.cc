// Grab our corresponding header for this class
#include "../include/primaryGeneratorAction.hh"

// G4 libraries needed
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

// Class constructor definition
OurPrimaryGeneratorAction::OurPrimaryGeneratorAction()
{
    // Boolean put in place to go between particle gun and source
    ifGeneralSource = true;
    
    if(ifGeneralSource == false){ 
        // Default behaviour: shoot gamma 1173 keV along Z axis.
        // Put any custom event generator here
        G4int n_particle = 1; // 1 per shot
        fParticleGun  = new G4ParticleGun(n_particle);

        // Properties
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4String particleName;
        G4ParticleDefinition* particle
        = particleTable->FindParticle(particleName="gamma");
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        fParticleGun->SetParticleEnergy(1.173*MeV);
  }
  else{
        // Else: use general particle source
        fGPS = new G4GeneralParticleSource;
  }
}

// Destructor
OurPrimaryGeneratorAction::~OurPrimaryGeneratorAction()
{
    delete fParticleGun; // Clean up old C pointers
    delete fGPS;
}

// Base G4 member function, but with our options put in
void OurPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Function is called at the begining of each event
    if(ifGeneralSource == false){ 
        // If particle gun
        fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
    else{
        // If GP source
        fGPS->GeneratePrimaryVertex(anEvent);
    }
}