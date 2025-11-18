#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include "DetectorConstruction.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

class DetectorConstructionMessenger: public G4UImessenger
{

	public:
		DetectorConstructionMessenger(DetectorConstruction*);
		~DetectorConstructionMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		DetectorConstruction* theDetector;

		G4UIdirectory* DetectorDir;

};

#endif

