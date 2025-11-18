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

#ifndef AngularDistribution_h
#define AngularDistribution_h 1

#include "G4RunManager.hh"
#include "AngularDistributionMessenger.hh"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "TGraph.h"
#include "TH1F.h"
#include "globals.hh"

#ifndef PI
#define PI (3.14159265358979)
#endif

#ifndef R2D
#define R2D (5.72957795130823229e+01)
#endif

#ifndef D2R
#define D2R (1.74532925199432955e-02)
#endif 

class AngularDistributionMessenger;

class AngularDistribution {
	public:
		AngularDistribution();    
		virtual ~AngularDistribution();
		AngularDistribution(const AngularDistribution& );
		AngularDistribution& operator = (const AngularDistribution&);

		void	ReadAngularDistribution(G4String);

		TH1F	*GetDistributionHistogram()	{ return hDist;	}
		G4bool	GoodDist() const		{ return bDist; }

	private:
		TGraph	*gDist;
		TH1F	*hDist;
		G4bool	bDist;

		AngularDistributionMessenger	*messenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
