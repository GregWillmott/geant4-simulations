#include "AngularDistributionMessenger.hh"

AngularDistributionMessenger::AngularDistributionMessenger(AngularDistribution* dist)
  	:theDist(dist)
{ 

	DistDir	= new G4UIdirectory("/distGen/");
	DistDir->SetGuidance("User generator action control."); 

	setDistFile	= new G4UIcmdWithAString("/distGen/setDistFile",this);
	setDistFile->SetGuidance("Set dist file");
	setDistFile->SetParameterName("choice",true);
	setDistFile->SetDefaultValue("None");
	setDistFile->AvailableForStates(G4State_PreInit,G4State_Idle);

}


AngularDistributionMessenger::~AngularDistributionMessenger()
{

	delete	DistDir;
	
	delete	setDistFile;

}


void AngularDistributionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 	

	if( command == setDistFile )
		theDist->ReadAngularDistribution(newValue);
  
}

