// Grab our corresponding header for this class
#include "../include/physics.hh"

// G4 libraries needed
#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsShielding.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonElasticPhysics.hh"
// High Precision neutron data
#include "G4NeutronCrossSectionXS.hh"
// #include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4IonPhysics.hh"

// LEND data can be downloaded and Linux environmental variable G4LENDDATA needs to give the location
// #include "G4HadronElasticPhysicsLEND.hh"
// #include "G4HadronPhysicsShieldingLEND.hh"


// Define class constructor. Expected args: verbosity
OurPhysics::OurPhysics(int verbosity) : G4VModularPhysicsList()
{
    SetVerboseLevel(verbosity);

    // --------------------
    // EM Physics
    // --------------------
    // Most detailed EM Phys package, available alternatives include:
    // G4EmLivermorePhysics, G4EmPenelopePhysics, G4EmStandardPhysics_option1, 
    RegisterPhysics(new G4EmStandardPhysics_option4(verbosity));

    //Synchroton Radiation & GN Physics
    // G4EmExtraPhysics* emExtraPhysics = new G4EmExtraPhysics(verbosity);
    // Use Livermore low-energy nuclear data for gamma-nuclear interactions
    // emExtraPhysics->LENDGammaNuclear(true);
    // RegisterPhysics(emExtraPhysics);

    //  Radioactive Physics
    RegisterPhysics(new G4DecayPhysics(verbosity));
    RegisterPhysics(new G4RadioactiveDecayPhysics(verbosity));

    // Hadron Elastic scattering
    // RegisterPhysics(new G4HadronElasticPhysicsLEND(verbosity));
    RegisterPhysics(new G4HadronElasticPhysicsHP(verbosity));

    // Hadron inelastic scattering
    // RegisterPhysics(new G4HadronPhysicsShieldingLEND(verbosity));
    RegisterPhysics(new G4HadronPhysicsShielding(verbosity));

    // Stopping Physics
    RegisterPhysics(new G4StoppingPhysics(verbosity));

    // Ion Physics
    RegisterPhysics(new G4IonElasticPhysics(verbosity));
    RegisterPhysics(new G4IonPhysics(verbosity));

    // Neutrons
    RegisterPhysics(new G4NeutronCrossSectionXS(verbosity));

    // Premade Hadron inelastic
    // RegisterPhysics(new G4HadronPhysicsFTFP_BERT(verbosity));

}

// Destructor
OurPhysics::~OurPhysics()
{}