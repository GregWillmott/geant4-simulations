#include "ActionInitialization.hh"


ActionInitialization::ActionInitialization(DetectorConstruction* detector, HistoManager *hist)
	:	G4VUserActionInitialization(), fDetector(detector), fHistograms(hist)
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{

	RunAction* runAction = new RunAction(fHistograms);
	SetUserAction(runAction);

}

void ActionInitialization::Build() const
{
	
	RunAction* runAction = new RunAction(fHistograms);
	SetUserAction(runAction);

	PrimaryGeneratorAction *pGen = new PrimaryGeneratorAction();
	SetUserAction(pGen);

	EventAction* eventAction = new EventAction(runAction,fHistograms);
	SetUserAction(eventAction);

	SetUserAction(new SteppingAction(eventAction));

}
