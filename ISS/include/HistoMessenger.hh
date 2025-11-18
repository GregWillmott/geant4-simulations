#ifndef HistoMessenger_h
#define HistoMessenger_h 1

#include "HistoManager.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

class HistoMessenger: public G4UImessenger
{

	public:
		HistoMessenger(HistoManager*);
		~HistoMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		HistoManager* theHistos;

		G4UIdirectory* HistoDir;

		G4UIcmdWithAString*		FileNameCmd;	

};

#endif

