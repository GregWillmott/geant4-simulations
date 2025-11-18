#ifndef AngularDistributionMessenger_h
#define AngularDistributionMessenger_h 1

#include "AngularDistribution.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

class AngularDistribution;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;


class AngularDistributionMessenger: public G4UImessenger
{

	public:
		AngularDistributionMessenger(AngularDistribution*);
		~AngularDistributionMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		AngularDistribution* 	theDist;

		G4UIdirectory* 		DistDir;

		G4UIcmdWithAString	*setDistFile;

};

#endif

