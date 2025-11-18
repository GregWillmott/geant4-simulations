#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "RunAction.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

class RunAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;


class RunActionMessenger: public G4UImessenger
{

	public:
		RunActionMessenger(RunAction*);
		~RunActionMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		RunAction* 		theRun;

		G4UIdirectory* 		RunDir;

};

#endif

