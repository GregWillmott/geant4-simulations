#include "DetectorHit.hh"
#include "G4VProcess.hh"
#include <iostream>
#include <fstream>

DetectorHit::DetectorHit(const DetectorHit& d){

   	detID		= d.detID;
 	EDep		= d.EDep;
   	EDepP		= d.EDepP;
   	ToF		= d.ToF;
	position	= d.position;
	detHit		= d.detHit;

}

DetectorHit& DetectorHit::operator =  (const DetectorHit& d){

   	detID		= d.detID;
 	EDep		= d.EDep;
   	EDepP		= d.EDepP;
   	ToF		= d.ToF;
	position	= d.position;
	detHit		= d.detHit;

	return *this;

}

DetectorHit::DetectorHit(const G4Step *step, int id){

	detID		= id;

	EDep		= 0;
	EDepP		= 0;
	ToF		= -1;

	detHit		= true;	

	G4double edepStep = (step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit()) * MeV;
	G4VPhysicalVolume* pVolume
		= step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	if(pVolume->GetName().contains("DetCan"))
		detHit = false;

	G4String pname 	= step->GetTrack()->GetParticleDefinition()->GetParticleName();
	EDep 		= edepStep;
	ToF 		= step->GetTrack()->GetGlobalTime() * ns; 
	position 	= G4ThreeVector();
	position.set(step->GetPostStepPoint()->GetPosition().x(),step->GetPostStepPoint()->GetPosition().y(),step->GetPostStepPoint()->GetPosition().z());
	if(pname.contains("proton")){
		EDepP 	= edepStep;
	}

}

void DetectorHit::AddStep(const G4Step *step){

	G4double edepStep = step->GetTotalEnergyDeposit() * MeV;

	G4String pname 	= step->GetTrack()->GetParticleDefinition()->GetParticleName();
	EDep 		+= edepStep;
	if(pname.contains("proton"))
		EDepP 	+= edepStep;

}
