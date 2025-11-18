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
// $Id: B1EventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class
#include "Distances.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
//#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction, HistoManager* histo)
: G4UserEventAction(),
  fRunAction(runAction),
  fHistoManager(histo)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* e)
{
	fHits.clear();
	EventEDep = 0.;
	Z = 0.;
	stripID = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* e)
{   
	const PrimaryGeneratorAction* generatorAction
		= static_cast<const PrimaryGeneratorAction*>
	(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
	G4String runCondition;
	G4double particleEnergy = 0.;
	G4double particleTheta = 0.;
	G4double particlePhi = 0.;
	G4double tCm = 0.;
	G4ThreeVector particlevec;
	if (generatorAction)
	{
		particleEnergy 	= generatorAction->GetParticleEnergy() * MeV;
		particleTheta	= generatorAction->GetParticleTheta();
		particlePhi	= generatorAction->GetParticlePhi();
		tCm		= generatorAction->GetCmTheta();
	}
	
	if(e->GetEventID() % 10000 == 0)
		std::cout	<< "Event " << e->GetEventID() << " of " << fRunAction->GetNevents() << "\r" << std::flush;

	if(fHits.size() == 0){ // if nothing
		fHistoManager->FillTree(particleEnergy,particleTheta,particlePhi,tCm);
		return;
	} // else
	fHistoManager->FillTree(particleEnergy,particleTheta,particlePhi,tCm,fHits);


	// Fill Greg histos
	// 1.)
	auto analysisManager = G4AnalysisManager::Instance();

	// * * * * *
	// What p strip was hit and what Z does this translate to?
	// * * * * *

	// Common to all:
	Z -= (end_d + target_to_array_end_cap + end_cap_to_strip);

	// What Module was hit?
	if(stripID < 512 && stripID >= 0){ // Mod 0
		// What ASIC was hit?
		if(stripID < 128){ // 0 -> 127 ASIC 0
			// 1st ASIC (most upstream p-side)
			Z -= (128 * 3 * pSide_d); // 3 full ASICs downstream of the one we're modelling here
			Z -= (3 * asic_to_asic_gap); // 3 gaps
			Z -= (127.5 - stripID)*pSide_d; // Strip 0 is the most upstream. Smaller number is further away. 127.5 so it's in the centre of a p strip
		}
		if(stripID > 127 && stripID < 256){ // 128 -> 255 ASIC 2
			// 2nd ASIC
			Z -= (128 * 2 * pSide_d);
			Z -= (2 * asic_to_asic_gap);
			Z -= (255.5 - stripID)*pSide_d;
		}
		if(stripID > 255 && stripID < 384){ // 256 -> 383 ASIC 3
			// 3rd ASIC
			Z -= (128 * 1 * pSide_d);
			Z -= (1 * asic_to_asic_gap);
			Z -= (383.5 - stripID)*pSide_d;
		}
		if(stripID > 383 && stripID < 512){ // 384 -> 511 ASIC 5
			// 4th ASIC (most downstream p-side)
			Z -= (511.5 - stripID)*pSide_d;
		}
	}
	// ***
	if(stripID >= 600 && stripID < 1112){ // Mod 1 (same as Mod 0 but +600 to IDs)
		// What ASIC was hit?
		if(stripID < 728){ // 600 -> 727 ASIC 0
			// 1st ASIC (most upstream p-side)
			Z -= (128 * 3 * pSide_d); // 3 full ASICs downstream of the one we're modelling here
			Z -= (3 * asic_to_asic_gap); // 3 gaps
			Z -= (727.5 - stripID)*pSide_d; // Strip 0 is the most upstream. Smaller number is further away
		}
		if(stripID > 727 && stripID < 856){ // 728 -> 855 ASIC 2
			// 2nd ASIC
			Z -= (128 * 2 * pSide_d);
			Z -= (2 * asic_to_asic_gap);
			Z -= (855.5 - stripID)*pSide_d;
		}
		if(stripID > 855 && stripID < 984){ // 856 -> 983 ASIC 3
			// 3rd ASIC
			Z -= (128 * 1 * pSide_d);
			Z -= (1 * asic_to_asic_gap);
			Z -= (983.5 - stripID)*pSide_d;
		}
		if(stripID > 983 && stripID < 1112){ // 984 -> 1111 ASIC 5
			// 4th ASIC (most downstream p-side)
			Z -= (1111.5 - stripID)*pSide_d;
		}
	}
	// ***
	if(stripID >= 1200){ // Mod 2 (same as Mod 0 but +1200 to IDs)
		// What ASIC was hit?
		if(stripID < 1328){ // 1200 -> 1327 ASIC 0
			// 1st ASIC (most upstream p-side)
			Z -= (128 * 3 * pSide_d); // 3 full ASICs downstream of the one we're modelling here
			Z -= (3 * asic_to_asic_gap); // 3 gaps
			Z -= (1327.5 - stripID)*pSide_d; // Strip 0 is the most upstream. Smaller number is further away
		}
		if(stripID > 1327 && stripID < 1456){ // 1328 -> 1455 ASIC 2
			// 2nd ASIC
			Z -= (128 * 2 * pSide_d);
			Z -= (2 * asic_to_asic_gap);
			Z -= (1455.5 - stripID)*pSide_d;
		}
		if(stripID > 1455 && stripID < 1584){ // 1456 -> 1583 ASIC 3
			// 3rd ASIC
			Z -= (128 * 1 * pSide_d);
			Z -= (1 * asic_to_asic_gap);
			Z -= (1583.5 - stripID)*pSide_d;
		}
		if(stripID > 1583 && stripID < 1712){ // 1584 -> 1711 ASIC 5
			// 4th ASIC (most downstream p-side)
			Z -= (1711.5 - stripID)*pSide_d;
		}
	}

	analysisManager->FillH2(0, Z, EventEDep);
	analysisManager->FillH2(1, Z, particleEnergy);

	// G4double T_cyc_ej = 2 * M_PI * 1 / (2.5 * 1);
	// G4double E_cm = EventEDep + 

	analysisManager->FillH1(0, particleEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::AddEDepCopyID(G4double dE, G4int ID){
	EventEDep += dE; // sum up step depositions to get the event total
	stripID = ID; // p strip the particle is hitting
}
