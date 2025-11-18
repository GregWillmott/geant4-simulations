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
// $Id: B1RunAction.cc 99560 2016-09-27 07:03:29Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
//#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager *histo)
: G4UserRunAction(),
  fHistoManager(histo)
{

	messenger	= new RunActionMessenger(this);

	// Get analysis manager instance
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);

	// Create histos
	G4int n_E_bins = 4096;
	G4int n_Z_bins = 1028;
	G4double EMax = 128*MeV;
	G4double ZMax = 1.*m;
	// Hist 1: Energy vs Z coord TH2
	analysisManager->CreateH2("EDep-vs-Z", "Energy dep (p-side) vs Z dist from target", n_Z_bins, -ZMax, 0., n_E_bins, 0., EMax);
	analysisManager->CreateH2("Proton-E0-vs-Z", "Initial proton energy vs Z dist from target", n_Z_bins, -ZMax, 0., n_E_bins, 0., EMax);
	analysisManager->CreateH1("Proton-EDep", "Proton-EDep", n_E_bins, 0., EMax);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* theRun)
{ 

	nbEventInRun = theRun->GetNumberOfEventToBeProcessed();

  	// inform the runManager to save random number seed
  	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	G4cout << "Initialisation complete, run beginning." << G4endl; // Marks construction of geometry etc and the start of transport

   	// Get instance of analysis manager
   	auto analysisManager = G4AnalysisManager::Instance();

    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%d-%m-%Y-%H-%M-%S", timeinfo);
    std::string timeStr(buffer.data());

    // Pick either of these lines for the name
    // G4String filename = "ISS-Sim" + G4String(timeStr) + ".root";
    G4String filename = "ISSTest.root";

    // For different formats change .root to .csv, .hdf5, .xml
    analysisManager->OpenFile(filename);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
	// Get instance of analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();

	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) return;
		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

