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
/// \file analysis/AnaEx01/include/HistoManager.hh
/// \brief Definition of the HistoManager class
//
//
// $Id: HistoManager.hh 99607 2016-09-28 13:33:42Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#ifndef HistoManager_h
#define HistoManager_h 1

#include "G4ThreeVector.hh"
#include "DetectorHit.hh"
#include <vector>
#include "globals.hh"

class TFile;
class TTree;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
	public:
		HistoManager();
		~HistoManager();     

		void 	FillTree(G4double, G4double, G4double, G4double, std::vector<DetectorHit>);
		void	FillTree(G4double, G4double, G4double, G4double);

		void	SetFilename(G4String str)	{ fFilename = str; }

		void 	Book();
		void 	Save(); 

		void	Clear();

	private:
		TFile*   	fRootFile;          
		TTree*   	fTree;   

		std::vector<double> 	f_Edep;
		std::vector<double> 	f_EdepP;
		double 		f_PrimE;
		double		f_PrimT;
		double		f_PrimP;
		double		f_Etot;
		double		f_tCm;
		std::vector<double>	f_ToF;
		std::vector<double>	f_Theta;
		std::vector<int>	f_detID;
		std::vector<double>	f_X;
		std::vector<double>	f_Y;
		std::vector<double>	f_Z;
		std::vector<double>	f_R;
		std::vector<bool>	f_detHit;

		G4String	fFilename;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
