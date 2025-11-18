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
/// \file analysis/AnaEx01/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
// $Id: HistoManager.cc 100674 2016-10-31 10:43:40Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


namespace {
        G4Mutex writemutex = G4MUTEX_INITIALIZER;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
 : 	fRootFile(0), 
 	fTree(0)
{
	fFilename = "Data.root";
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{

	G4cout	<< "Making tree" << G4endl;

  	G4String fileName = fFilename;
  	fRootFile = new TFile(fileName,"RECREATE");
  	if (! fRootFile) {
    		G4cout 	<< " HistoManager::Book :" 
           		<< " problem creating the ROOT TFile "
           		<< G4endl;
    		return;
  	}

  	// create 1st ntuple
  	fTree = new TTree("DataTree","DataTree");
  	fTree->Branch("PrimE", &f_PrimE);
  	fTree->Branch("PrimT", &f_PrimT);
  	fTree->Branch("PrimP", &f_PrimP);
	fTree->Branch("ThetaCM", &f_tCm);
	fTree->Branch("Etot", &f_Etot);
	fTree->Branch("Edep", &f_Edep);
  	fTree->Branch("EdepP", &f_EdepP);
	fTree->Branch("ToF", &f_ToF);
	fTree->Branch("DetID", &f_detID);
	fTree->Branch("Theta", &f_Theta);
	fTree->Branch("X", &f_X);
	fTree->Branch("Y", &f_Y);
	fTree->Branch("Z", &f_Z);
	fTree->Branch("R", &f_R);
	fTree->Branch("DetectorHit", &f_detHit);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{

   	if (! fRootFile) return;
  
  	fRootFile->Write();       // Writing the histograms to the file
  	fRootFile->Close();       // and closing the tree (and the file)
  
	G4cout << "\n----> Tree saved\n" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillTree(G4double primE, G4double primT, G4double primP, G4double tCm,
			std::vector<DetectorHit> hits)
{                

	{
		G4AutoLock l(&writemutex);
	
		Clear();

		f_PrimE = primE;
		f_PrimT = primT;
		f_PrimP = primP;
		f_tCm	= tCm;
		for(size_t i=0;i<hits.size();i++){
			f_Edep.push_back(hits.at(i).GetEDep());
			f_EdepP.push_back(hits.at(i).GetEDepP());
			f_ToF.push_back(hits.at(i).GetToF());
			f_detID.push_back(hits.at(i).GetDetID());
			f_X.push_back(hits.at(i).GetPosition().x());
			f_Y.push_back(hits.at(i).GetPosition().y());
			f_Z.push_back(hits.at(i).GetPosition().z());
			f_R.push_back(hits.at(i).GetPosition().r());
			f_Theta.push_back(hits.at(i).GetPosition().theta());
			f_detHit.push_back(hits.at(i).IsDetHit());
			f_Etot += hits.at(i).GetEDep();
		}

		if(f_Edep.size() == 0)
			return;

		if (fTree){ 
			fTree->Fill();	
		}
		Clear();
	}

}  

void HistoManager::FillTree(G4double primE, G4double primT, G4double primP, G4double tCm){

	{
		G4AutoLock l(&writemutex);
		Clear();
		f_PrimE = primE;
		f_PrimT = primT;
		f_PrimP = primP;
		f_tCm	= tCm;
		if(fTree){
			fTree->Fill();	
		}
		Clear();
	}
}

void HistoManager::Clear(){

	f_Edep.clear();
	f_EdepP.clear();
	f_PrimE = 0;
	f_PrimT = 0;
	f_PrimP = 0;
	f_tCm	= 0;
	f_Etot	= 0;
	f_ToF.clear();
	f_detID.clear();
	f_X.clear();
	f_Y.clear();
	f_Z.clear();
	f_R.clear();
	f_Theta.clear();
	f_detHit.clear();

}
