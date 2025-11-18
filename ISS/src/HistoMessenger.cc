#include "HistoMessenger.hh"

HistoMessenger::HistoMessenger(HistoManager* histos)
  	:theHistos(histos)
{ 
	// directories 
	HistoDir = new G4UIdirectory("/Histograms/");
	HistoDir->SetGuidance("Histogram control.");

	// commands
	FileNameCmd = new G4UIcmdWithAString("/Histograms/SetFilename",this);
	FileNameCmd->SetGuidance("Histogram filename");
	FileNameCmd->SetParameterName("choice",false);
	FileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}


HistoMessenger::~HistoMessenger()
{

	// directories
	delete HistoDir;

	// commands
	delete FileNameCmd;

}


void HistoMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 	

	if( command == FileNameCmd )
		theHistos->SetFilename(newValue);
	  
}

