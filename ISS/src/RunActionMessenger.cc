#include "RunActionMessenger.hh"

RunActionMessenger::RunActionMessenger(RunAction* run)
  	:theRun(run)
{ 

	RunDir	= new G4UIdirectory("/runGen/");
	RunDir->SetGuidance("User run action control."); 


}


RunActionMessenger::~RunActionMessenger()
{

	delete	RunDir;

}


void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 	
  
}

