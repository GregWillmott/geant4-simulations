#include "DetectorConstructionMessenger.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* Det)
  	:theDetector(Det)
{ 
	// directories 
	DetectorDir = new G4UIdirectory("/DetectorConstruction/");
	DetectorDir->SetGuidance("Detector construction control.");

}


DetectorConstructionMessenger::~DetectorConstructionMessenger()
{

	// directories
	delete DetectorDir;

}


void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 	
	  
}

