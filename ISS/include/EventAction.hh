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
// $Id: B1EventAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1EventAction.hh
/// \brief Definition of the B1EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4ThreeVector.hh"
#include "G4UserEventAction.hh"
#include "DetectorHit.hh"
#include "globals.hh"
#include <chrono>

class RunAction;
class HistoManager;

/// Event action class
///

class EventAction : public G4UserEventAction
{
	public:
		EventAction(RunAction*, HistoManager*);
		virtual ~EventAction();
		//EventAction(const EventAction& d);
		//EventAction& operator = (const EventAction& d);
		G4double EventEDep = 0.;
		G4double Z = 0.;
		G4int stripID = 0;

		virtual void BeginOfEventAction(const G4Event* event) override;
		virtual void EndOfEventAction(const G4Event* event) override;
		std::vector<DetectorHit>	GetHits() const { return fHits;		}
		void	SetHit(DetectorHit h, int i)		{ fHits.at(i) = h;	}
		void	AddHit(DetectorHit h)			{ fHits.push_back(h);	}
		void    AddEDepCopyID(G4double, G4int);

	private:
		RunAction* fRunAction;
		HistoManager* fHistoManager;	
		std::vector<DetectorHit>	fHits;	

		std::chrono::system_clock::time_point	start;
		std::chrono::system_clock::time_point	end;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
