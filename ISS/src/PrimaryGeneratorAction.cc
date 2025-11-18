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
// $Id: B1PrimaryGeneratorAction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{

	messenger		= new PrimaryGeneratorMessenger(this);

	fDist			= new AngularDistribution();

	G4int n_particle 	= 1;
	fParticleSource 	= new G4GeneralParticleSource();
	fParticleGun		= new G4ParticleGun();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete	fParticleSource;
  	delete 	fParticleGun;

	delete	fDist;

	delete	messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//this function is called at the begining of each event


	if(true){

		fKin = Kinematics(beamA,beamZ,targetA,targetZ,ejectileA,ejectileZ,ExcE);

		int	recoilZ	= beamZ + targetZ - ejectileZ;
		int	recoilA	= beamA + targetA - ejectileA;

		double	incEnergy = (double)(beamEnergy - beamEnergyLoss*G4UniformRand()) * MeV;

		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4IonTable* ionTable = G4IonTable::GetIonTable();
		G4String particleName;
		G4ParticleDefinition* particle
			= particleTable->FindParticle(particleName="proton");

		double	randR	= G4UniformRand() * beamSpotSize;
		double	randP	= G4UniformRand() * 2. * PI;

		double	randX	= randR * sin(randP);
		double	randY	= randR * cos(randP);

		G4PrimaryVertex *g4vtx;
		G4PrimaryParticle *g4prim;

		fKin.SetReactionEnergy(incEnergy);
		//fKin.CalculateKinematics(ExcE);

		if(fDist->GoodDist()){
			tCm	= fDist->GetDistributionHistogram()->GetRandom() * D2R;
		}
		else{
			tCm	= acos(2*G4UniformRand() - 1.);
		}


		vert = fKin.CreateVertex(tCm);

		double	etLab	= vert.GetEjecTheta();
		double	epLab	= G4UniformRand() * 2. * PI;
		g4prim = new G4PrimaryParticle();
		g4prim->SetKineticEnergy(vert.GetEjecKinE() * MeV);
		// g4prim->SetKineticEnergy(12.*MeV);
		g4prim->SetMomentumDirection(G4ThreeVector(sin(etLab)*cos(epLab),sin(etLab)*sin(epLab),cos(etLab)));
		g4prim->SetG4code(particleTable->FindParticle(particleName="proton"));
		g4vtx = new G4PrimaryVertex(randX,randY,0.,0.);
		g4vtx->SetPrimary(g4prim);
		anEvent->AddPrimaryVertex(g4vtx);	

		double	rtLab	= vert.GetRecoilTheta();
		double	rpLab	= epLab - PI;
		g4prim = new G4PrimaryParticle();
		g4prim->SetKineticEnergy(vert.GetRecoilKinE() * MeV);
		g4prim->SetMomentumDirection(G4ThreeVector(sin(rtLab)*cos(rpLab),sin(rtLab)*sin(rpLab),cos(rtLab)));
		g4prim->SetG4code(G4IonTable::GetIonTable()->GetIon(recoilZ,recoilA,ExcE));
		g4vtx = new G4PrimaryVertex(randX,randY,0.,0.);
		g4vtx->SetPrimary(g4prim);
		anEvent->AddPrimaryVertex(g4vtx);

		rEnergy	= vert.GetRecoilKinE() * MeV;
		rTheta	= rtLab;
		rPhi	= rpLab;
		eEnergy	= vert.GetEjecKinE() * MeV;
		eTheta	= etLab;
		ePhi	= epLab;

	}
	else{
		fParticleSource->GeneratePrimaryVertex(anEvent);
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(const PrimaryGeneratorAction& v){

	messenger = v.messenger;

	fParticleGun	= v.fParticleGun; // pointer a to G4 gun class
	fParticleSource = v.fParticleSource; // pointer a to G4 gun class
	fKin 		= v.fKin;
	vert 		= v.vert;

	rEnergy 	= v.rEnergy;
	rTheta 		= v.rTheta;
	rPhi 		= v.rPhi;
	eEnergy 	= v.eEnergy;
	eTheta 		= v.eTheta;
	ePhi 		= v.ePhi;

	beamSpotSize 	= v.beamSpotSize;
	beamEnergy 	= v.beamEnergy;
	beamEnergyLoss 	= v.beamEnergyLoss;

	ExcE 		= v.ExcE;

	beamA 		= v.beamA;
	beamZ 		= v.beamZ;
	targetA 	= v.targetA;
	targetZ 	= v.targetZ;
	ejectileA 	= v.ejectileA;
	ejectileZ 	= v.ejectileZ;

	bDist 		= v.bDist;

	fDist 		= v.fDist;

}
PrimaryGeneratorAction& PrimaryGeneratorAction::operator = (const PrimaryGeneratorAction& v){
	

	messenger = v.messenger;

	fParticleGun	= v.fParticleGun; // pointer a to G4 gun class
	fParticleSource = v.fParticleSource; // pointer a to G4 gun class
	fKin 		= v.fKin;
	vert 		= v.vert;

	rEnergy 	= v.rEnergy;
	rTheta 		= v.rTheta;
	rPhi 		= v.rPhi;
	eEnergy 	= v.eEnergy;
	eTheta 		= v.eTheta;
	ePhi 		= v.ePhi;

	beamSpotSize 	= v.beamSpotSize;
	beamEnergy 	= v.beamEnergy;
	beamEnergyLoss 	= v.beamEnergyLoss;

	ExcE 		= v.ExcE;

	beamA 		= v.beamA;
	beamZ 		= v.beamZ;
	targetA 	= v.targetA;
	targetZ 	= v.targetZ;
	ejectileA 	= v.ejectileA;
	ejectileZ 	= v.ejectileZ;

	bDist 		= v.bDist;

	fDist 		= v.fDist;	

	return *this;

}
