#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "PrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

class PrimaryGeneratorAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;


class PrimaryGeneratorMessenger: public G4UImessenger
{

	public:
		PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
		~PrimaryGeneratorMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		PrimaryGeneratorAction* 	theGenerator;

		G4UIdirectory* 			GeneratorDir;


		G4UIcmdWithAnInteger*	setBeamA;
		G4UIcmdWithAnInteger*	setBeamZ;
		G4UIcmdWithAnInteger*	setTargetA;
		G4UIcmdWithAnInteger*	setTargetZ;
		G4UIcmdWithAnInteger*	setEjectileA;
		G4UIcmdWithAnInteger*	setEjectileZ;

		G4UIcmdWithADoubleAndUnit*	setBeamSpotSize;
		G4UIcmdWithADoubleAndUnit*	setBeamEnergy;
		G4UIcmdWithADoubleAndUnit*	setBeamEnergyLoss;
		G4UIcmdWithADoubleAndUnit*	setExcitationEnergy;

};

#endif

