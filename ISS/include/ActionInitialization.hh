#ifndef ActionInitialization_hh
#define ActionInitialization_hh

#include "G4VUserActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

class ActionInitialization : public G4VUserActionInitialization {

	public:
		ActionInitialization(DetectorConstruction*, HistoManager*);
		virtual ~ActionInitialization();
	
		virtual	void	BuildForMaster() const;
		virtual void	Build() const;	

	private:
		DetectorConstruction*	fDetector;
		HistoManager*		fHistograms;
		AngularDistribution*	fDist;

}; 

#endif
