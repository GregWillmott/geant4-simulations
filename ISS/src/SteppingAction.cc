//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include <chrono>
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcommand.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

	// get volume of the current step
	G4LogicalVolume* volume 
		= step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4VPhysicalVolume* pVolume
		= step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

	// if(!volume || !pVolume)
		// return;

	if(volume->GetMaterial()->GetName() != G4String("p_Si")) // only want p-side hits
		return;

	G4double edepStep = step->GetTotalEnergyDeposit() * MeV; // keV -> MeV
	G4int silicon_ID = step->GetPostStepPoint()->GetTouchable()->GetCopyNumber();

	if(edepStep > 0){
		/* JH
		// Create detID str
		int detID = 0;
		if(pVolume->GetName().contains("Chamber"))
			detID = -1;
		else if(pVolume->GetName().contains("Detector")){
			std::string str = pVolume->GetName();
			std::size_t const n = str.find_first_of("0123456789");
			G4String idstr = "";
			for(size_t i = n; i < str.length(); i++)
				idstr.append(str[i]);
			detID = G4UIcommand::ConvertToInt(idstr);
			if(pVolume->GetName().contains("Stop")){
				detID += 10;
			}
		}

		// Construct hit
		bool test = true;
		for(size_t h = 0; h < fEventAction->GetHits().size(); h++){
			if(fEventAction->GetHits().at(h).GetDetID() == detID 		// This detector ID exists
			&& ((fEventAction->GetHits().at(h).IsDetHit() && test)		// It is a detector and the stored hit is a detector OR
			|| (!fEventAction->GetHits().at(h).IsDetHit() && !test)) ){	// it ISN'T a detector and the stored hit ISN'T a detector
				DetectorHit hit = fEventAction->GetHits().at(h);
				hit.AddStep(step);
				fEventAction->SetHit(hit,h);
				return;
			}
		}
		*/

		// Create detID str
		int detID = 0;
		detID = pVolume->GetCopyNo(); // G4int type

		// Construct hit
		for(size_t h = 0; h < fEventAction->GetHits().size(); h++){
			if(fEventAction->GetHits().at(h).GetDetID() == detID){ // if eventaction[i].hit is this hit
				DetectorHit hit = fEventAction->GetHits().at(h);
				hit.AddStep(step);
				fEventAction->SetHit(hit,h);
				return;
			}
		}

		DetectorHit hit(step,detID); // write the DetectorHit, and ID integer to custom class
		fEventAction->AddHit(hit); // put that class object into an array
		
		// Greg function
		G4String p_name = "proton";
		if(step->GetTrack()->GetParticleDefinition()->GetParticleName() != p_name) return;
		fEventAction->AddEDepCopyID(edepStep, silicon_ID);
		return;

	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

