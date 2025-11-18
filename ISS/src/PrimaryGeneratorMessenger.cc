#include "PrimaryGeneratorMessenger.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* gen)
  	:theGenerator(gen)
{ 

	GeneratorDir	= new G4UIdirectory("/actionGen/");
	GeneratorDir->SetGuidance("User generator action control."); 

	setBeamA	= new G4UIcmdWithAnInteger("/actionGen/setBeamA",this);
	setBeamA->SetGuidance("Set beam A");
	setBeamA->SetParameterName("choice",true);
	setBeamA->SetDefaultValue((G4int)21);
	setBeamA->AvailableForStates(G4State_PreInit,G4State_Idle);

	setBeamZ	= new G4UIcmdWithAnInteger("/actionGen/setBeamZ",this);
	setBeamZ->SetGuidance("Set beam Z");
	setBeamZ->SetParameterName("choice",true);
	setBeamZ->SetDefaultValue((G4int)10);
	setBeamZ->AvailableForStates(G4State_PreInit,G4State_Idle);

	setTargetA	= new G4UIcmdWithAnInteger("/actionGen/setTargetA",this);
	setTargetA->SetGuidance("Set target A");
	setTargetA->SetParameterName("choice",true);
	setTargetA->SetDefaultValue((G4int)2);
	setTargetA->AvailableForStates(G4State_PreInit,G4State_Idle);

	setTargetZ	= new G4UIcmdWithAnInteger("/actionGen/setTargetZ",this);
	setTargetZ->SetGuidance("Set target Z");
	setTargetZ->SetParameterName("choice",true);
	setTargetZ->SetDefaultValue((G4int)1);
	setTargetZ->AvailableForStates(G4State_PreInit,G4State_Idle);

	setEjectileA	= new G4UIcmdWithAnInteger("/actionGen/setEjectileA",this);
	setEjectileA->SetGuidance("Set beam A");
	setEjectileA->SetParameterName("choice",true);
	setEjectileA->SetDefaultValue((G4int)1);
	setEjectileA->AvailableForStates(G4State_PreInit,G4State_Idle);

	setEjectileZ	= new G4UIcmdWithAnInteger("/actionGen/setEjectileZ",this);
	setEjectileZ->SetGuidance("Set beam A");
	setEjectileZ->SetParameterName("choice",true);
	setEjectileZ->SetDefaultValue((G4int)1);
	setEjectileZ->AvailableForStates(G4State_PreInit,G4State_Idle);

	setBeamSpotSize	= new G4UIcmdWithADoubleAndUnit("/actionGen/setBeamSpotSize",this);
	setBeamSpotSize->SetGuidance("Set beam spot size");
	setBeamSpotSize->SetParameterName("choice",true);
	setBeamSpotSize->SetDefaultValue((G4double)0);
	setBeamSpotSize->AvailableForStates(G4State_PreInit,G4State_Idle);

	setBeamEnergy	= new G4UIcmdWithADoubleAndUnit("/actionGen/setBeamEnergy",this);
	setBeamEnergy->SetGuidance("Set beam energy");
	setBeamEnergy->SetParameterName("choice",true);
	setBeamEnergy->SetDefaultValue((G4double)1000.*MeV);
	setBeamEnergy->AvailableForStates(G4State_PreInit,G4State_Idle);

	setBeamEnergyLoss	= new G4UIcmdWithADoubleAndUnit("/actionGen/setBeamEnergyLoss",this);
	setBeamEnergyLoss->SetGuidance("Set beam energy loss");
	setBeamEnergyLoss->SetParameterName("choice",true);
	setBeamEnergyLoss->SetDefaultValue((G4double)0.*MeV);
	setBeamEnergyLoss->AvailableForStates(G4State_PreInit,G4State_Idle);

	setExcitationEnergy	= new G4UIcmdWithADoubleAndUnit("/actionGen/setExcitationEnergy",this);
	setExcitationEnergy->SetGuidance("Set excitation energy");
	setExcitationEnergy->SetParameterName("choice",true);
	setExcitationEnergy->SetDefaultValue((G4double)0.*MeV);
	setExcitationEnergy->AvailableForStates(G4State_PreInit,G4State_Idle);

}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{

	delete	GeneratorDir;
	

	delete	setBeamA;
	delete	setBeamZ;
	delete	setTargetA;
	delete	setTargetZ;
	delete	setEjectileA;
	delete	setEjectileZ;

	delete	setBeamSpotSize;
	delete	setBeamEnergy;
	delete	setBeamEnergyLoss;

}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 	

	if( command == setBeamSpotSize )
		theGenerator->SetBeamSpotSize(setBeamSpotSize->GetNewDoubleValue(newValue));
	if( command == setBeamEnergy )
		theGenerator->SetBeamEnergy(setBeamEnergy->GetNewDoubleValue(newValue));
	if( command == setBeamEnergyLoss )
		theGenerator->SetBeamEnergyLoss(setBeamEnergyLoss->GetNewDoubleValue(newValue));
	if( command == setBeamA )
		theGenerator->SetBeamA(setBeamA->GetNewIntValue(newValue));
	if( command == setBeamZ )
		theGenerator->SetBeamZ(setBeamZ->GetNewIntValue(newValue));
	if( command == setTargetA )
		theGenerator->SetTargetA(setTargetA->GetNewIntValue(newValue));
	if( command == setTargetZ )
		theGenerator->SetTargetZ(setTargetZ->GetNewIntValue(newValue));
	if( command == setEjectileA )
		theGenerator->SetEjectileA(setEjectileA->GetNewIntValue(newValue));
	if( command == setEjectileZ )
		theGenerator->SetEjectileZ(setEjectileZ->GetNewIntValue(newValue));
	if( command == setExcitationEnergy )
		theGenerator->SetExcitationEnergy(setExcitationEnergy->GetNewDoubleValue(newValue));
	  
}

