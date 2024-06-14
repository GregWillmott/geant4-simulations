// Grab our corresponding headers for this class
#include "../include/actionInitialisation.hh"

#include "../include/primaryGeneratorAction.hh"
#include "../include/runAction.hh"
#include "../include/eventAction.hh"
#include "../include/steppingAction.hh"
#include "../include/detectorConstruction.hh"

// Constructor: pointer to the geometry class object is set up so we can use it in the stepping action.
OurActionInitialization::OurActionInitialization(OurDetectorConstruction* detectorCon)
 : fDetectorCon(detectorCon)
{}

// Functions from Geant, have to put in definitions to other classes
void OurActionInitialization::BuildForMaster() const
{
    SetUserAction(new OurRunAction);
}

void OurActionInitialization::Build() const
{
    SetUserAction(new OurPrimaryGeneratorAction);

    SetUserAction(new OurRunAction);

    OurEventAction* eventAction = new OurEventAction();
    SetUserAction(eventAction);

    SetUserAction(new OurSteppingAction(fDetectorCon,eventAction));
}