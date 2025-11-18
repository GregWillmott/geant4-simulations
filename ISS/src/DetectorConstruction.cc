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
// $Id: B5DetectorConstruction.cc 77656 2013-11-27 08:52:57Z gcosmo $
//
/// \file B5DetectorConstruction.cc
/// \brief Implementation of the B5DetectorConstruction class

#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4Polyhedra.hh"


//#include "G4GDMLParser.hh"
#include "Distances.hh"
#include "DetectorConstruction.hh"
#include <math.h>
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: 	G4VUserDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	DefineMiscGeometry();
	DefineMaterials();

	///////////
	// World
	///////////
	G4double world_sizeXY = 2*m;
	G4double world_sizeZ  = 6*m;

	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;
	
  	G4Box	*solidWorld =    
    		new G4Box("World",                       //its name
	       	0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  	G4LogicalVolume	*logicWorld =                         
    		new G4LogicalVolume(solidWorld,         //its solid
                world_mat,           		 	//its material
                "World");  			        //its name
                                   
  	G4VPhysicalVolume	*physWorld
    		= new G4PVPlacement(0,          //no rotation
                G4ThreeVector(),       		//at (0,0,0)
                logicWorld,            		//its logical volume
                "World",               		//its name
                0,                     		//its mother  volume
                false,                 		//no boolean operation
                0,                     		//copy number
                checkOverlaps);        		//overlaps checking

	// Z Rotations
	G4RotationMatrix* rotZ30 = new G4RotationMatrix();
	G4RotationMatrix* rotZ60 = new G4RotationMatrix();
	G4RotationMatrix* rotZ90 = new G4RotationMatrix();
	G4RotationMatrix* rotZ120 = new G4RotationMatrix();
	G4RotationMatrix* rotZ150 = new G4RotationMatrix();
	G4RotationMatrix* rotZ180 = new G4RotationMatrix();
	G4RotationMatrix* rotZ210 = new G4RotationMatrix();
	G4RotationMatrix* rotZ240 = new G4RotationMatrix();
	G4RotationMatrix* rotZ270 = new G4RotationMatrix();
	G4RotationMatrix* rotZ300 = new G4RotationMatrix();
	G4RotationMatrix* rotZ330 = new G4RotationMatrix();
	rotZ30->rotateZ(30.*deg);
	rotZ60->rotateZ(60.*deg);
	rotZ90->rotateZ(90.*deg);
	rotZ120->rotateZ(120.*deg);
	rotZ150->rotateZ(150.*deg);
	rotZ180->rotateZ(180.*deg);
	rotZ210->rotateZ(210.*deg);
	rotZ240->rotateZ(240.*deg);
	rotZ270->rotateZ(270.*deg);
	rotZ300->rotateZ(300.*deg);
	rotZ330->rotateZ(330.*deg);


	/////////
	// Field
	/////////
	// Parameters
	fieldStrength = 2.45*tesla;
	G4double field_r = 60.*cm;
	G4double field_d = 260.*cm;

	solidField = new G4Tubs("Field", 0, field_r, 0.5*field_d, 0, 360.*deg); // name, r0, r1, half-Z, angle start, ang range
	logicField = new G4LogicalVolume(solidField, world_mat, "Field");
	new G4PVPlacement(0, G4ThreeVector(), logicField, "Field", logicWorld, false, 0, checkOverlaps);

	//////////////
	// Target
	//////////////
	// Parameters
	G4double frame_h = 54.*mm;
	G4double frame_w = 14.*mm;
	G4double frame_d = 0.33*mm;
	G4double frame_rad = 5.*mm;
	G4RotationMatrix* rot_frame = new G4RotationMatrix();
	rot_frame->rotateZ(90.*deg);

	G4double target_thickness = 207.*1E-6; // g/cm2
	G4double cd2_density = 1.08; // g/cm3

	G4double target_d = (target_thickness / cd2_density)*cm; // cm
	
	// Target frame
	// Solid vols
	G4Box* solidTargetBox = new G4Box("Target-box", 0.5*frame_h, 0.5*frame_w, 0.5*frame_d); // name, x, y, z (half lengths)
	G4Tubs* solidTargetHole = new G4Tubs("Target-hole", 0., frame_rad, frame_d, 0., 360.*deg); // name, r_inner, r_outer, half Z, start phi, phi range
	G4SubtractionSolid* solidTargetFrame = new G4SubtractionSolid("Target-frame", solidTargetBox, solidTargetHole, G4Transform3D()); // name, subject, bit-to-subtract, orientation
	// Logical and placement
	G4LogicalVolume* logicTargetFrame = new G4LogicalVolume(solidTargetFrame, Al, "Target-frame");
	new G4PVPlacement(rot_frame, G4ThreeVector(), logicTargetFrame, "Target-frame", logicField, false, 0, checkOverlaps);

	// Target
	G4Tubs* solidTarget = new G4Tubs("Target", 0., frame_rad, 0.5*target_d, 0., 360.*deg);
	G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, CD2, "Target");
	new G4PVPlacement(0, G4ThreeVector(), logicTarget, "Target", logicField, false, 0, checkOverlaps);


	/////////////////////////
	// Beam Tube and Al ends
	/////////////////////////
	// Parameters

	G4double beam_t_r0 = 9.525*mm; // r: radius
	G4double beam_t_r1 = 11.525*mm;
	G4double beam_t_d = 515.*mm; // d: depth
	G4double end_r0 = 9.525*mm;
	G4double end_r1 = 30./1.1547*mm;
	G4double array_mid_Z = -(target_to_array_end_cap+0.5*(beam_t_d)+end_d); // target to array + end cap + half the tube

	G4ThreeVector pos_array_midpoint = G4ThreeVector(0,0,array_mid_Z);
	G4ThreeVector end_centre_to_BT_centre = G4ThreeVector(0,0,0.5*(beam_t_d+end_d));

	// Main tube
	// Solid vols
	G4Tubs* solidBeamTube = new G4Tubs("Beam-tube", beam_t_r0, beam_t_r1, 0.5*beam_t_d, 0, 360.*deg);
	G4Tubs* solidBTEnd = new G4Tubs("Beam-tube-end", end_r0, end_r1, 0.5*end_d, 0, 360.*deg);
	G4UnionSolid* solidBeamTube2 = new G4UnionSolid("Beam-tube-plus-one-end", solidBeamTube, solidBTEnd, 0, end_centre_to_BT_centre); // name, subject, bit-to-subtract, rotation matrix, 3Vector
	// G4UnionSolid* solidBeamTube3 = new G4UnionSolid("Beam-tube-plus-two-ends", solidBeamTube2, solidBTEnd, 0, -end_centre_to_BT_centre); // name, subject, bit-to-subtract, rotation matrix, 3Vector

	G4LogicalVolume* logicBeamTube = new G4LogicalVolume(solidBeamTube2, Al, "Beam-tube");
	new G4PVPlacement(0, pos_array_midpoint, logicBeamTube, "Beam-tube", logicField, false, 0, checkOverlaps);

	//////////////////
	// Array supports
	//////////////////
	// Parameters
	G4double hex_side_l = 31.118*mm;
	G4double zHexPlanes[] = {-beam_t_d*0.5, beam_t_d*0.5};
	G4double rZeroes[] = {0,0};
	G4double rHex[] = {hex_side_l/1.1547,hex_side_l/1.1547};

	// Box cutout 1
	G4double boxCutout_w = 100.*mm;
	G4double boxCutout_h = 50.*mm;
	G4double boxCutout_d = beam_t_d;
	G4double posX = 13.5*cos(M_PI/6) - 0.5*boxCutout_h*cos(M_PI/6);
	G4double posY = 13.5*sin(M_PI/6) - 0.5*boxCutout_h*sin(M_PI/6);

	// G4ThreeVector posBoxCutout = G4ThreeVector((9.95929)*mm, (-5.75-boxCutout_h/2.)*mm, 0); // From trig
	G4ThreeVector posBoxCutout = G4ThreeVector(posX, posY, 0);
	G4RotationMatrix* rotBoxCutout = new G4RotationMatrix();
	rotBoxCutout->rotateZ(60.*deg);

	// Cylinder cutout
	G4double cutCyl_r0 = 1.81*mm;
	G4double cutCyl_r1 = 5.*mm;

	posX = (13.5+1.81)*cos(M_PI/6) - (23.762*cos(M_PI/3));
	posY = (13.5+1.81)*sin(M_PI/6) + (23.762*sin(M_PI/3));

	G4ThreeVector posCylCutout = G4ThreeVector(posX, posY, 0);
	G4RotationMatrix* rotCylCutout = new G4RotationMatrix();
	rotCylCutout->rotateZ(300.*deg);

	// Box cutout 2
	G4double boxCutout2_w = 30.*mm;
	G4double boxCutout2_h = 12.*mm;
	G4double boxCutout2_d = beam_t_d;
	posX = (31.118)*cos(M_PI/6) - (0.5*boxCutout2_h*sin(22.5*M_PI/180.));
	posY = -(31.118)*sin(M_PI/6) - (0.5*boxCutout2_h*cos(22.5*M_PI/180.)) + 2*mm; // extra number to put the plane where it is supposed to be: not on the hex corner

	G4ThreeVector posBoxCutout2 = G4ThreeVector(posX, posY, 0);
	G4RotationMatrix* rotBoxCutout2 = new G4RotationMatrix();
	rotBoxCutout2->rotateZ(22.5*deg);

	// Solid vols
	G4Polyhedra* solidHex = new G4Polyhedra("Hexagon", 30.*deg, 360.*deg, 6, 2, zHexPlanes, rZeroes, rHex);
	G4Box* solidCutBox = new G4Box("Cutout-box", 0.5*boxCutout_w, 0.5*boxCutout_h, 0.5*boxCutout_d+(1.*mm));
	G4SubtractionSolid* solidHex2 = new G4SubtractionSolid("Hexagon2", solidHex, solidCutBox, rotBoxCutout, posBoxCutout);

	G4Tubs* solidCutCyl = new G4Tubs("Cutout-cyl", cutCyl_r0, cutCyl_r1, 0.5*boxCutout_d+(1.*mm), 0.*deg, 140.*deg); // extra 1mm on Z so Z plane isn't shared
	G4SubtractionSolid* solidHex3 = new G4SubtractionSolid("Hexagon3", solidHex2, solidCutCyl, rotCylCutout, posCylCutout);

	G4Box* solidCutBox2 = new G4Box("Cutout-box2", 0.5*boxCutout2_w, 0.5*boxCutout2_h, 0.5*boxCutout2_d+(1.*mm));
	G4SubtractionSolid* solidHex4 = new G4SubtractionSolid("Hexagon4", solidHex3, solidCutBox2, rotBoxCutout2, posBoxCutout2);

	G4LogicalVolume* logicHex = new G4LogicalVolume(solidHex4, Al, "Hexagon");
	new G4PVPlacement(rotZ60, pos_array_midpoint, logicHex, "Hexagon", logicField, false, 0, checkOverlaps); // 0 deg + 60 to get realistic orientation

	new G4PVPlacement(rotZ180, pos_array_midpoint, logicHex, "Hexagon", logicField, false, 0, checkOverlaps); // 120+60 deg
	new G4PVPlacement(rotZ300, pos_array_midpoint, logicHex, "Hexagon", logicField, false, 0, checkOverlaps); // 240+60 deg


	//////////////////
	// Magnet and bed
	//////////////////
	// Parameters
	G4double chamber_r0 = 45.75*cm;
	G4double chamber_r1 = 55.75*cm;
	G4double chamber_d = 250.*cm; // depth
	G4double bed_w = 50.*cm;
	G4double bed_h = 2.*cm;
	G4ThreeVector posBed = G4ThreeVector(0, -chamber_r0+(10.*cm)-(0.5*bed_h), 0);

	// Volumes
	G4Tubs* solidMagnet = new G4Tubs("Magnet", chamber_r0, chamber_r1, 0.5*chamber_d, 0, 360.*deg);
	G4LogicalVolume* logicMagnet = new G4LogicalVolume(solidMagnet, Al,"Magnet");
	new G4PVPlacement(0, G4ThreeVector(), logicMagnet, "Magnet", logicField, false, 0, checkOverlaps);

	G4Box* solidBed = new G4Box("Bed", 0.5*bed_w, 0.5*bed_h, 0.5*chamber_d);
	G4LogicalVolume* logicBed = new G4LogicalVolume(solidBed, Al, "Bed");
	new G4PVPlacement(0, posBed, logicBed, "Bed", logicField, false, 0, checkOverlaps);


	//////////////////////
	// Silicon Array Base
	//////////////////////
	// Parameters
	G4double backing_w = 28.5*mm;
	G4double backing_h = 1.*mm;
	G4double backing_d = 501.5*mm;

	G4double backing2_w = 2.5*mm;
	G4double backing2_h = 1*mm;
	G4double backing2_d = backing_d*mm;

	G4double backing3_w = 22*mm;
	G4double backing3_h = 1*mm;
	G4double backing3_d = 5.*mm;

	G4ThreeVector pos_back2_left = G4ThreeVector(-0.5*backing_w + 0.5*backing2_w +1.5*mm, 0.5*backing_h+0.5*backing2_h, 0);
	G4ThreeVector pos_back2_right = G4ThreeVector(0.5*backing_w - 0.5*backing2_w, 0.5*backing_h+0.5*backing2_h, 0);

	G4ThreeVector pos_back3_front = G4ThreeVector(0.5*1.5*mm, 0.5*backing_h+0.5*backing3_h, backing3_d+121.5*mm); // 'rungs' of the ladder
	G4ThreeVector pos_back3_mid = G4ThreeVector(0.5*1.5*mm, 0.5*backing_h+0.5*backing3_h, 0);
	G4ThreeVector pos_back3_back = G4ThreeVector(0.5*1.5*mm, 0.5*backing_h+0.5*backing3_h, -backing3_d-121.5*mm);

	posX = (31.118/1.1547)*mm + 0.5*backing_h;
	posY = -0.75*mm; // The backings don't sit in the middle of the hex
	G4ThreeVector posBacking = G4ThreeVector(posX, -posY, array_mid_Z);

	// Volumes 1 & 2 (+y dir)
	G4Box* solidBacking = new G4Box("Backing", 0.5*backing_w, 0.5*backing_h, 0.5*backing_d);

	// Add the 5 extra boxes to make a 'ladder' shape so the Si can fit in the gaps
	G4Box* solidBacking2 = new G4Box("Backing2", 0.5*backing2_w, 0.5*backing2_h, 0.5*backing2_d);
	G4Box* solidBacking3 = new G4Box("Backing3", 0.5*backing3_w, 0.5*backing3_h, 0.5*backing3_d);
	G4MultiUnion* solidBackingFinal = new G4MultiUnion("BackingFinal");
	solidBackingFinal->AddNode(solidBacking, G4Transform3D(G4RotationMatrix(), G4ThreeVector())); // add original volume
	solidBackingFinal->AddNode(solidBacking2, G4Transform3D(G4RotationMatrix(), pos_back2_left)); // add left long box
	solidBackingFinal->AddNode(solidBacking2, G4Transform3D(G4RotationMatrix(), pos_back2_right)); // add right
	solidBackingFinal->AddNode(solidBacking3, G4Transform3D(G4RotationMatrix(), pos_back3_front)); // add dividers
	solidBackingFinal->AddNode(solidBacking3, G4Transform3D(G4RotationMatrix(), pos_back3_mid));
	solidBackingFinal->AddNode(solidBacking3, G4Transform3D(G4RotationMatrix(), pos_back3_back));
	solidBackingFinal->Voxelize();

	G4LogicalVolume* logicBacking = new G4LogicalVolume(solidBackingFinal, Plastic, "Backing");
	// Backings 1,3,5
	// Rotate 60 deg
	posBacking.rotateZ(60.*deg);
	new G4PVPlacement(rotZ30, posBacking, logicBacking, "Backing", logicField, false, 0, checkOverlaps);

	posBacking.rotateZ(120.*deg);
	new G4PVPlacement(rotZ270, posBacking, logicBacking, "Backing", logicField, false, 0, checkOverlaps); // 30+60 deg

	posBacking.rotateZ(120.*deg);
	new G4PVPlacement(rotZ150, posBacking, logicBacking, "Backing", logicField, false, 0, checkOverlaps); // 90+60 deg

	// Backings 2,4,6
	// The two halves of each module are a reflection of each other. Reuse solidBackingFinal, but rotate X 180
	G4ThreeVector posBacking2 = G4ThreeVector(posX, posY, array_mid_Z); // y shift also in other direction
	G4RotationMatrix* rotZ270X180 = new G4RotationMatrix();
	rotZ270X180->rotateZ(270.*deg); rotZ270X180->rotateX(180.*deg);
	new G4PVPlacement(rotZ270X180, posBacking2, logicBacking, "Backing", logicField, false, 0, checkOverlaps);

	posBacking2.rotateZ(120.*deg);
	G4RotationMatrix* rotZ150X180 = new G4RotationMatrix();
	rotZ150X180->rotateZ(150.*deg); rotZ150X180->rotateX(180.*deg);
	new G4PVPlacement(rotZ150X180, posBacking2, logicBacking, "Backing", logicField, false, 0, checkOverlaps);

	posBacking2.rotateZ(120.*deg);
	G4RotationMatrix* rotZ30X180 = new G4RotationMatrix();
	rotZ30X180->rotateZ(30.*deg); rotZ30X180->rotateX(180.*deg);
	new G4PVPlacement(rotZ30X180, posBacking2, logicBacking, "Backing", logicField, false, 0, checkOverlaps);


	//////////////////////
	// Module electronics
	//////////////////////
	// Approximation for the boards + casing on the ASIC 1 side of each module
	// Parameters
	G4double electr_w = 90.*mm;
	G4double electr_h = 17.*mm;
	G4double electr_d = 120.*mm;

	G4Box* solid_electr = new G4Box("Electronics", 0.5*electr_w, 0.5*electr_h, 0.5*electr_d);
	G4LogicalVolume* logic_electr = new G4LogicalVolume(solid_electr, Al, "Electronics");

	G4ThreeVector pos_electr = G4ThreeVector((0.5*electr_w - (31.118/1.1547)*mm),((31.118+3.)*mm - 0.5*electr_h),(array_mid_Z - 0.5*beam_t_d - 0.5*electr_d));
	new G4PVPlacement(0, pos_electr, logic_electr, "Electronics", logicField, false, 0, checkOverlaps);

	pos_electr.rotateZ(120.*deg);
	new G4PVPlacement(rotZ60, pos_electr, logic_electr, "Electronics", logicField, false, 0, checkOverlaps);
	pos_electr.rotateZ(120.*deg);
	new G4PVPlacement(rotZ120, pos_electr, logic_electr, "Electronics", logicField, false, 0, checkOverlaps);

	/////////////////
	// Silicon Array
	/////////////////
	// Parameters
	G4double nSide_w = 0.5*mm;
	G4double nSide_h = 2.*mm;
	G4double nSide_d = 121.5*mm;

	G4double mod0_ch11_midZ;
	G4double mod0_ch88_midZ;
	G4double x_si_start = (31.118/1.1547)*mm + 1.0*backing_h; // where backing turns to Si on x-axis

	G4double mod1_ch11_midZ;
	G4double mod1_ch88_midZ;

	G4Box* solidNSide = new G4Box("Array-n-side", 0.5*nSide_w, 0.5*nSide_h, 0.5*nSide_d); // these are common to Mod 0,1,2 so they're outside the loop
	G4LogicalVolume* logicNSide = new G4LogicalVolume(solidNSide, n_Si, "Array-n-side");

	/////////////////////
	// Module 0 - n side
	/////////////////////
	//////////
	// ASIC 1
	//////////
	// ********** ASIC 1 Ch 11 - 21 **********
	G4ThreeVector posAsic1Ch11 = G4ThreeVector(x_si_start+0.5*nSide_w, - 5*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d); // Ch 11
	posAsic1Ch11.rotateZ(60.*deg);
	fSiliconNSidePV = new G4PVPlacement(rotZ120, posAsic1Ch11, logicNSide, "Array-n-side", logicField, false, 11, checkOverlaps); // 11 = copy num

	G4ThreeVector posAsic1Ch11to21;
	for(int i=1; i<11; i++){ // 1 to 10 inc.
		posAsic1Ch11to21 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d);
		posAsic1Ch11to21.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic1Ch11to21, logicNSide, "Array-n-side", logicField, false, 11+i, checkOverlaps);
	}

	// ********** ASIC 1 Ch 28 - 38 **********
	G4ThreeVector posAsic1Ch28to38;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch28to38 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d);
		posAsic1Ch28to38.rotateZ(120.*deg);
		new G4PVPlacement(rotZ60, posAsic1Ch28to38, logicNSide, "Array-n-side", logicField, false, 28+i, checkOverlaps); // 28 to 38
	}

	// ********** ASIC 1 Ch 89 - 99 **********
	G4ThreeVector posAsic1Ch89to99;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch89to99 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z - 0.5*asic_to_asic_gap - 0.5*nSide_d);
		posAsic1Ch89to99.rotateZ(120.*deg);
		new G4PVPlacement(rotZ60, posAsic1Ch89to99, logicNSide, "Array-n-side", logicField, false, 89+i, checkOverlaps); // 89 to 99
	}

	// ********** ASIC 1 Ch 106 - 116 **********
	G4ThreeVector posAsic1Ch106to116;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch106to116 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z - 0.5*asic_to_asic_gap - 0.5*nSide_d);
		posAsic1Ch106to116.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic1Ch106to116, logicNSide, "Array-n-side", logicField, false, 106+i, checkOverlaps); // 106 to 116
	}

	//////////
	// ASIC 4
	//////////

	// ********** ASIC 4 Ch 11 - 28 ***********
	G4ThreeVector posAsic4Ch11to21;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch11to21 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z + 0.5*asic_to_asic_gap + 0.5*nSide_d); // Z is different to ASIC 1
		posAsic4Ch11to21.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic4Ch11to21, logicNSide, "Array-n-side", logicField, false, 200+11+i, checkOverlaps); // +200 to ID so no clashes with ASIC 1
	}

	// ********** ASIC 4 Ch 28 - 38 **********
	G4ThreeVector posAsic4Ch28to38;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch28to38 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z + 0.5*asic_to_asic_gap + 0.5*nSide_d);
		posAsic4Ch28to38.rotateZ(120.*deg);
		new G4PVPlacement(rotZ60, posAsic4Ch28to38, logicNSide, "Array-n-side", logicField, false, 200+28+i, checkOverlaps); // 28 to 38
	}

	// ********** ASIC 4 Ch 89 - 99 **********
	G4ThreeVector posAsic4Ch89to99;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch89to99 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z + 1.5*asic_to_asic_gap + 1.5*nSide_d);
		posAsic4Ch89to99.rotateZ(120.*deg);
		new G4PVPlacement(rotZ60, posAsic4Ch89to99, logicNSide, "Array-n-side", logicField, false, 200+89+i, checkOverlaps); // 89 to 99
	}

	// ********** ASIC 4 Ch 106 - 116 **********
	G4ThreeVector posAsic4Ch106to116;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch106to116 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z + 1.5*asic_to_asic_gap + 1.5*nSide_d);
		posAsic4Ch106to116.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic4Ch106to116, logicNSide, "Array-n-side", logicField, false, 200+106+i, checkOverlaps); // 106 to 116
	}

	/////////////////////
	// Module 1 - n side
	/////////////////////
	//////////
	// ASIC 1
	//////////
	// ********** ASIC 1 Ch 11 - 21 **********
	G4ThreeVector posAsic1Ch11to21Mod1;
	for(int i=0; i<11; i++){ // 1 to 10 inc.
		posAsic1Ch11to21Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d);
		posAsic1Ch11to21Mod1.rotateZ(180.*deg); // 60+120 deg
		new G4PVPlacement(0, posAsic1Ch11to21Mod1, logicNSide, "Array-n-side", logicField, false, 400+11+i, checkOverlaps); // +400 ID difference from Mod 0
	}

	// ********** ASIC 1 Ch 28 - 38 **********
	G4ThreeVector posAsic1Ch28to38Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch28to38Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d);
		posAsic1Ch28to38Mod1.rotateZ(240.*deg); // 120+120 deg
		new G4PVPlacement(rotZ300, posAsic1Ch28to38Mod1, logicNSide, "Array-n-side", logicField, false, 400+28+i, checkOverlaps); // 28 to 38. 300 deg = -60
	}

	// ********** ASIC 1 Ch 89 - 99 **********
	G4ThreeVector posAsic1Ch89to99Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch89to99Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z - 0.5*asic_to_asic_gap - 0.5*nSide_d);
		posAsic1Ch89to99Mod1.rotateZ(240.*deg); // 120+120 deg
		new G4PVPlacement(rotZ300, posAsic1Ch89to99Mod1, logicNSide, "Array-n-side", logicField, false, 400+89+i, checkOverlaps); // 89 to 99
	}

	// ********** ASIC 1 Ch 106 - 116 **********
	G4ThreeVector posAsic1Ch106to116Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch106to116Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z - 0.5*asic_to_asic_gap - 0.5*nSide_d);
		posAsic1Ch106to116Mod1.rotateZ(180.*deg); // 60+120 deg
		new G4PVPlacement(0, posAsic1Ch106to116Mod1, logicNSide, "Array-n-side", logicField, false, 400+106+i, checkOverlaps); // 106 to 116
	}

	//////////
	// ASIC 4
	//////////

	// ********** ASIC 4 Ch 11 - 28 ***********
	G4ThreeVector posAsic4Ch11to21Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch11to21Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z + 0.5*asic_to_asic_gap + 0.5*nSide_d); // Z is different to ASIC 1
		posAsic4Ch11to21Mod1.rotateZ(180.*deg); // 60+120 deg
		new G4PVPlacement(0, posAsic4Ch11to21Mod1, logicNSide, "Array-n-side", logicField, false, 400+200+11+i, checkOverlaps); // +200 to ID so no clashes with ASIC 1
	}

	// ********** ASIC 4 Ch 28 - 38 **********
	G4ThreeVector posAsic4Ch28to38Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch28to38Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z + 0.5*asic_to_asic_gap + 0.5*nSide_d);
		posAsic4Ch28to38Mod1.rotateZ(240.*deg); // 120+120 deg
		new G4PVPlacement(rotZ300, posAsic4Ch28to38Mod1, logicNSide, "Array-n-side", logicField, false, 400+200+28+i, checkOverlaps); // 28 to 38
	}

	// ********** ASIC 4 Ch 89 - 99 **********
	G4ThreeVector posAsic4Ch89to99Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch89to99Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z + 1.5*asic_to_asic_gap + 1.5*nSide_d);
		posAsic4Ch89to99Mod1.rotateZ(240.*deg); // 120+120 deg
		new G4PVPlacement(rotZ300, posAsic4Ch89to99Mod1, logicNSide, "Array-n-side", logicField, false, 400+200+89+i, checkOverlaps); // 89 to 99
	}

	// ********** ASIC 4 Ch 106 - 116 **********
	G4ThreeVector posAsic4Ch106to116Mod1;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch106to116Mod1 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z + 1.5*asic_to_asic_gap + 1.5*nSide_d);
		posAsic4Ch106to116Mod1.rotateZ(180.*deg); // 60+120 deg
		new G4PVPlacement(0, posAsic4Ch106to116Mod1, logicNSide, "Array-n-side", logicField, false, 400+200+106+i, checkOverlaps); // 106 to 116
	}


	/////////////////////
	// Module 2 - n side
	/////////////////////
	//////////
	// ASIC 1
	//////////
	// ********** ASIC 1 Ch 11 - 21 **********
	G4ThreeVector posAsic1Ch11to21Mod2;
	for(int i=0; i<11; i++){ // 1 to 10 inc.
		posAsic1Ch11to21Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d);
		posAsic1Ch11to21Mod2.rotateZ(300.*deg); // 60+240 deg
		new G4PVPlacement(rotZ60, posAsic1Ch11to21Mod2, logicNSide, "Array-n-side", logicField, false, 800+11+i, checkOverlaps); // +400 ID difference from Mod 0
	}

	// ********** ASIC 1 Ch 28 - 38 **********
	G4ThreeVector posAsic1Ch28to38Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch28to38Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z - 1.5*asic_to_asic_gap - 1.5*nSide_d);
		posAsic1Ch28to38Mod2.rotateZ(0.*deg); // 120+240 deg
		new G4PVPlacement(0, posAsic1Ch28to38Mod2, logicNSide, "Array-n-side", logicField, false, 800+28+i, checkOverlaps); // 28 to 38. 300 deg = -60
	}

	// ********** ASIC 1 Ch 89 - 99 **********
	G4ThreeVector posAsic1Ch89to99Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch89to99Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z - 0.5*asic_to_asic_gap - 0.5*nSide_d);
		posAsic1Ch89to99Mod2.rotateZ(0.*deg); // 120+240 deg
		new G4PVPlacement(0, posAsic1Ch89to99Mod2, logicNSide, "Array-n-side", logicField, false, 800+89+i, checkOverlaps); // 89 to 99
	}

	// ********** ASIC 1 Ch 106 - 116 **********
	G4ThreeVector posAsic1Ch106to116Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic1Ch106to116Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z - 0.5*asic_to_asic_gap - 0.5*nSide_d);
		posAsic1Ch106to116Mod2.rotateZ(300.*deg); // 60+240 deg
		new G4PVPlacement(rotZ60, posAsic1Ch106to116Mod2, logicNSide, "Array-n-side", logicField, false, 800+106+i, checkOverlaps); // 106 to 116
	}

	//////////
	// ASIC 4
	//////////

	// ********** ASIC 4 Ch 11 - 28 ***********
	G4ThreeVector posAsic4Ch11to21Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch11to21Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z + 0.5*asic_to_asic_gap + 0.5*nSide_d); // Z is different to ASIC 1
		posAsic4Ch11to21Mod2.rotateZ(300.*deg); // 60+240 deg
		new G4PVPlacement(rotZ60, posAsic4Ch11to21Mod2, logicNSide, "Array-n-side", logicField, false, 800+200+11+i, checkOverlaps); // +200 to ID so no clashes with ASIC 1
	}

	// ********** ASIC 4 Ch 28 - 38 **********
	G4ThreeVector posAsic4Ch28to38Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch28to38Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (-5+i)*nSide_h, array_mid_Z + 0.5*asic_to_asic_gap + 0.5*nSide_d);
		posAsic4Ch28to38Mod2.rotateZ(0.*deg); // 120+240 deg
		new G4PVPlacement(0, posAsic4Ch28to38Mod2, logicNSide, "Array-n-side", logicField, false, 800+200+28+i, checkOverlaps); // 28 to 38
	}

	// ********** ASIC 4 Ch 89 - 99 **********
	G4ThreeVector posAsic4Ch89to99Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch89to99Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z + 1.5*asic_to_asic_gap + 1.5*nSide_d);
		posAsic4Ch89to99Mod2.rotateZ(0.*deg); // 120+240 deg
		new G4PVPlacement(0, posAsic4Ch89to99Mod2, logicNSide, "Array-n-side", logicField, false, 800+200+89+i, checkOverlaps); // 89 to 99
	}

	// ********** ASIC 4 Ch 106 - 116 **********
	G4ThreeVector posAsic4Ch106to116Mod2;
	for(int i=0; i<11; i++){ // 0 to 10 inc.
		posAsic4Ch106to116Mod2 = G4ThreeVector(x_si_start+0.5*nSide_w, (5-i)*nSide_h, array_mid_Z + 1.5*asic_to_asic_gap + 1.5*nSide_d);
		posAsic4Ch106to116Mod2.rotateZ(300.*deg); // 60+240 deg
		new G4PVPlacement(rotZ60, posAsic4Ch106to116Mod2, logicNSide, "Array-n-side", logicField, false, 800+200+106+i, checkOverlaps); // 106 to 116
	}

	// Parameters
	G4double dead_layer_w = 400*1E-6*mm; // nm  literature 0.4 - 0.8 um
	G4double pSide_w = 0.5*mm - dead_layer_w; // x
	G4double pSide_h = 22.*mm; // y

	G4Box* solidPSide = new G4Box("Array-p-side", 0.5*pSide_w, 0.5*pSide_h, 0.5*pSide_d);
	G4LogicalVolume* logicPSide = new G4LogicalVolume(solidPSide, p_Si, "Array-p-side");


	/////////////////////
	// Module 0 - p side
	/////////////////////
	//////////
	// ASIC 0
	//////////
	// ********** ASIC 0 Ch 0 **********
	G4ThreeVector posAsic0Ch0 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 2.0*nSide_d - 1.5*asic_to_asic_gap + (0.5)*pSide_d));
	posAsic0Ch0.rotateZ(60.*deg);
	fSiliconPSidePV = new G4PVPlacement(rotZ120, posAsic0Ch0, logicPSide, "Array-p-side", logicField, false, 0, checkOverlaps);
	G4ThreeVector posAsic0Ch0_2 = posAsic0Ch0;
	posAsic0Ch0_2.rotateZ(60.*deg); // 60+60 deg
	new G4PVPlacement(rotZ60, posAsic0Ch0_2, logicPSide, "Array-p-side", logicField, false, 0, checkOverlaps);

	// ********** ASIC 0 Ch 1 - 127 **********
	G4ThreeVector posAsic0Ch1to127;
	G4ThreeVector posAsic0Ch1to127_2;
	for(int i=1; i<128; i++){ // 1 to 127
		// 1st half
		posAsic0Ch1to127 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 2.0*nSide_d - 1.5*asic_to_asic_gap + (0.5+i)*pSide_d));
		posAsic0Ch1to127.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic0Ch1to127, logicPSide, "Array-p-side", logicField, false, i, checkOverlaps);
		// 2nd half
		posAsic0Ch1to127_2 = posAsic0Ch1to127;
		posAsic0Ch1to127_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ60, posAsic0Ch1to127_2, logicPSide, "Array-p-side", logicField, false, i, checkOverlaps);
	}

	// ********** ASIC 2 Ch 0 - 127 **********
	G4ThreeVector posAsic2Ch1to127;
	G4ThreeVector posAsic2Ch1to127_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic2Ch1to127 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 1.0*nSide_d - 0.5*asic_to_asic_gap + (0.5+i)*pSide_d)); // further +Z than ASIC 0
		posAsic2Ch1to127.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic2Ch1to127, logicPSide, "Array-p-side", logicField, false, i+128, checkOverlaps); // ID +128 to not clash with ASIC 0
		// 2nd half
		posAsic2Ch1to127_2 = posAsic2Ch1to127;
		posAsic2Ch1to127_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ60, posAsic2Ch1to127_2, logicPSide, "Array-p-side", logicField, false, i+128, checkOverlaps);
	}

	// ********** ASIC 3 Ch 0 - 127 **********
	G4ThreeVector posAsic3Ch1to127;
	G4ThreeVector posAsic3Ch1to127_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic3Ch1to127 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 0.0*nSide_d + 0.5*asic_to_asic_gap + (0.5+i)*pSide_d)); // further +Z than ASIC 2
		posAsic3Ch1to127.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic3Ch1to127, logicPSide, "Array-p-side", logicField, false, i+256, checkOverlaps); // ID +128 to not clash with ASIC 2
		// 2nd half
		posAsic3Ch1to127_2 = posAsic3Ch1to127;
		posAsic3Ch1to127_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ60, posAsic3Ch1to127_2, logicPSide, "Array-p-side", logicField, false, i+256, checkOverlaps);
	}

	// ********** ASIC 5 Ch 0 - 127 **********
	G4ThreeVector posAsic5Ch1to127;
	G4ThreeVector posAsic5Ch1to127_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic5Ch1to127 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z + 1.0*nSide_d + 1.5*asic_to_asic_gap + (0.5+i)*pSide_d));
		posAsic5Ch1to127.rotateZ(60.*deg);
		new G4PVPlacement(rotZ120, posAsic5Ch1to127, logicPSide, "Array-p-side", logicField, false, i+384, checkOverlaps);
		// 2nd half
		posAsic5Ch1to127_2 = posAsic5Ch1to127;
		posAsic5Ch1to127_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ60, posAsic5Ch1to127_2, logicPSide, "Array-p-side", logicField, false, i+384, checkOverlaps);
	}


	/////////////////////
	// Module 1 - p side
	/////////////////////
	//////////
	// ASIC 0
	//////////
	// ********** ASIC 0 Ch 0 - 127 **********
	G4ThreeVector posAsic0Ch1to127Mod1;
	G4ThreeVector posAsic0Ch1to127Mod1_2;
	for(int i=0; i<128; i++){ // 0 to 127
		// 1st half
		posAsic0Ch1to127Mod1 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 2.0*nSide_d - 1.5*asic_to_asic_gap + (0.5+i)*pSide_d));
		posAsic0Ch1to127Mod1.rotateZ(180.*deg); // 60+120
		new G4PVPlacement(0, posAsic0Ch1to127Mod1, logicPSide, "Array-p-side", logicField, false, i+600, checkOverlaps); // +600 to not clash with Mod 0
		// 2nd half
		posAsic0Ch1to127Mod1_2 = posAsic0Ch1to127Mod1;
		posAsic0Ch1to127Mod1_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ300, posAsic0Ch1to127Mod1_2, logicPSide, "Array-p-side", logicField, false, i+600, checkOverlaps);
	}

	// ********** ASIC 2 Ch 0 - 127 **********
	G4ThreeVector posAsic2Ch1to127Mod1;
	G4ThreeVector posAsic2Ch1to127Mod1_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic2Ch1to127Mod1 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 1.0*nSide_d - 0.5*asic_to_asic_gap + (0.5+i)*pSide_d)); // further +Z than ASIC 0
		posAsic2Ch1to127Mod1.rotateZ(180.*deg);
		new G4PVPlacement(0, posAsic2Ch1to127Mod1, logicPSide, "Array-p-side", logicField, false, i+128+600, checkOverlaps); // ID +128 to not clash with ASIC 0
		// 2nd half
		posAsic2Ch1to127Mod1_2 = posAsic2Ch1to127Mod1;
		posAsic2Ch1to127Mod1_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ300, posAsic2Ch1to127Mod1_2, logicPSide, "Array-p-side", logicField, false, i+128+600, checkOverlaps);
	}

	// ********** ASIC 3 Ch 0 - 127 **********
	G4ThreeVector posAsic3Ch1to127Mod1;
	G4ThreeVector posAsic3Ch1to127Mod1_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic3Ch1to127Mod1 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 0.0*nSide_d + 0.5*asic_to_asic_gap + (0.5+i)*pSide_d)); // further +Z than ASIC 2
		posAsic3Ch1to127Mod1.rotateZ(180.*deg);
		new G4PVPlacement(0, posAsic3Ch1to127Mod1, logicPSide, "Array-p-side", logicField, false, i+256+600, checkOverlaps); // ID +128 to not clash with ASIC 2
		// 2nd half
		posAsic3Ch1to127Mod1_2 = posAsic3Ch1to127Mod1;
		posAsic3Ch1to127Mod1_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ300, posAsic3Ch1to127Mod1_2, logicPSide, "Array-p-side", logicField, false, i+256+600, checkOverlaps);
	}

	// ********** ASIC 5 Ch 0 - 127 **********
	G4ThreeVector posAsic5Ch1to127Mod1;
	G4ThreeVector posAsic5Ch1to127Mod1_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic5Ch1to127Mod1 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z + 1.0*nSide_d + 1.5*asic_to_asic_gap + (0.5+i)*pSide_d));
		posAsic5Ch1to127Mod1.rotateZ(180.*deg);
		new G4PVPlacement(0, posAsic5Ch1to127Mod1, logicPSide, "Array-p-side", logicField, false, i+384+600, checkOverlaps);
		// 2nd half
		posAsic5Ch1to127Mod1_2 = posAsic5Ch1to127Mod1;
		posAsic5Ch1to127Mod1_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ300, posAsic5Ch1to127Mod1_2, logicPSide, "Array-p-side", logicField, false, i+384+600, checkOverlaps);
	}

	/////////////////////
	// Module 2 - p side
	/////////////////////
	//////////
	// ASIC 0
	//////////
	// ********** ASIC 0 Ch 0 - 127 **********
	G4ThreeVector posAsic0Ch1to127Mod2;
	G4ThreeVector posAsic0Ch1to127Mod2_2;
	for(int i=0; i<128; i++){ // 0 to 127
		// 1st half
		posAsic0Ch1to127Mod2 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 2.0*nSide_d - 1.5*asic_to_asic_gap + (0.5+i)*pSide_d));
		posAsic0Ch1to127Mod2.rotateZ(300.*deg); // 60+120+120
		new G4PVPlacement(rotZ240, posAsic0Ch1to127Mod2, logicPSide, "Array-p-side", logicField, false, i+1200, checkOverlaps); // +600+600 to not clash with Mod 1
		// 2nd half
		posAsic0Ch1to127Mod2_2 = posAsic0Ch1to127Mod2;
		posAsic0Ch1to127Mod2_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ180, posAsic0Ch1to127Mod2_2, logicPSide, "Array-p-side", logicField, false, i+1200, checkOverlaps);
	}

	// ********** ASIC 2 Ch 0 - 127 **********
	G4ThreeVector posAsic2Ch1to127Mod2;
	G4ThreeVector posAsic2Ch1to127Mod2_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic2Ch1to127Mod2 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 1.0*nSide_d - 0.5*asic_to_asic_gap + (0.5+i)*pSide_d)); // further +Z than ASIC 0
		posAsic2Ch1to127Mod2.rotateZ(300.*deg);
		new G4PVPlacement(rotZ240, posAsic2Ch1to127Mod2, logicPSide, "Array-p-side", logicField, false, i+128+1200, checkOverlaps); // ID +128 to not clash with ASIC 0
		// 2nd half
		posAsic2Ch1to127Mod2_2 = posAsic2Ch1to127Mod2;
		posAsic2Ch1to127Mod2_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ180, posAsic2Ch1to127Mod2_2, logicPSide, "Array-p-side", logicField, false, i+128+1200, checkOverlaps);
	}

	// ********** ASIC 3 Ch 0 - 127 **********
	G4ThreeVector posAsic3Ch1to127Mod2;
	G4ThreeVector posAsic3Ch1to127Mod2_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic3Ch1to127Mod2 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z - 0.0*nSide_d + 0.5*asic_to_asic_gap + (0.5+i)*pSide_d)); // further +Z than ASIC 2
		posAsic3Ch1to127Mod2.rotateZ(300.*deg);
		new G4PVPlacement(rotZ240, posAsic3Ch1to127Mod2, logicPSide, "Array-p-side", logicField, false, i+256+1200, checkOverlaps); // ID +128 to not clash with ASIC 2
		// 2nd half
		posAsic3Ch1to127Mod2_2 = posAsic3Ch1to127Mod2;
		posAsic3Ch1to127Mod2_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ180, posAsic3Ch1to127Mod2_2, logicPSide, "Array-p-side", logicField, false, i+256+1200, checkOverlaps);
	}

	// ********** ASIC 5 Ch 0 - 127 **********
	G4ThreeVector posAsic5Ch1to127Mod2;
	G4ThreeVector posAsic5Ch1to127Mod2_2;
	for(int i=0; i<128; i++){ // 1 to 127
		// 1st half
		posAsic5Ch1to127Mod2 = G4ThreeVector((x_si_start + nSide_w + 0.5* pSide_w), 0, (array_mid_Z + 1.0*nSide_d + 1.5*asic_to_asic_gap + (0.5+i)*pSide_d));
		posAsic5Ch1to127Mod2.rotateZ(300.*deg);
		new G4PVPlacement(rotZ240, posAsic5Ch1to127Mod2, logicPSide, "Array-p-side", logicField, false, i+384+1200, checkOverlaps);
		// 2nd half
		posAsic5Ch1to127Mod2_2 = posAsic5Ch1to127Mod2;
		posAsic5Ch1to127Mod2_2.rotateZ(60.*deg);
		new G4PVPlacement(rotZ180, posAsic5Ch1to127Mod2_2, logicPSide, "Array-p-side", logicField, false, i+384+1200, checkOverlaps);
	}

	//////////////
	// Dead layer
	//////////////
	G4Box* solidDL = new G4Box("Dead-layer", 0.5*dead_layer_w, 0.5*pSide_h, 0.5*nSide_d);
	G4LogicalVolume* logicDL = new G4LogicalVolume(solidDL, Al, "Dead-layer"); // material is just a copy of silicon, but will allow hits to be differentiated from the strips

	// ASIC 0 coverage
	G4ThreeVector posDL1 = G4ThreeVector((x_si_start + nSide_w + pSide_w + 0.5*dead_layer_w), 0, (array_mid_Z - 1.5*nSide_d - 1.5*asic_to_asic_gap));
	posDL1.rotateZ(60.*deg);
	new G4PVPlacement(rotZ300, posDL1, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	G4ThreeVector posDL2; // Using one vector and overwriting it for each part of the hex bugs out, so one vector per side needed
	G4ThreeVector posDL3;
	G4ThreeVector posDL4;
	G4ThreeVector posDL5;
	G4ThreeVector posDL6;
	posDL2 = posDL1;
	posDL2.rotateZ(60.*deg);
	posDL3 = posDL1;
	posDL3.rotateZ(120.*deg);
	posDL4 = posDL1;
	posDL4.rotateZ(180.*deg);
	posDL5 = posDL1;
	posDL5.rotateZ(240.*deg);
	posDL6 = posDL1;
	posDL6.rotateZ(300.*deg);
	new G4PVPlacement(rotZ240, posDL2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ180, posDL3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ120, posDL4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ60, posDL5, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(0, posDL6, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);

	// ASIC 2 coverage
	G4ThreeVector posDL1_2 = G4ThreeVector((x_si_start + nSide_w + pSide_w + 0.5*dead_layer_w), 0, (array_mid_Z - 0.5*nSide_d - 0.5*asic_to_asic_gap));
	posDL1_2.rotateZ(60.*deg);
	new G4PVPlacement(rotZ300, posDL1_2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	G4ThreeVector posDL2_2; // Using one vector and overwriting it for each part of the hex bugs out, so one vector per side needed
	G4ThreeVector posDL3_2;
	G4ThreeVector posDL4_2;
	G4ThreeVector posDL5_2;
	G4ThreeVector posDL6_2;
	posDL2_2 = posDL1_2;
	posDL2_2.rotateZ(60.*deg);
	posDL3_2 = posDL1_2;
	posDL3_2.rotateZ(120.*deg);
	posDL4_2 = posDL1_2;
	posDL4_2.rotateZ(180.*deg);
	posDL5_2 = posDL1_2;
	posDL5_2.rotateZ(240.*deg);
	posDL6_2 = posDL1_2;
	posDL6_2.rotateZ(300.*deg);
	new G4PVPlacement(rotZ240, posDL2_2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ180, posDL3_2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ120, posDL4_2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ60, posDL5_2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(0, posDL6_2, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);

	// ASIC 3 coverage
	G4ThreeVector posDL1_3 = G4ThreeVector((x_si_start + nSide_w + pSide_w + 0.5*dead_layer_w), 0, (array_mid_Z + 0.5*nSide_d + 0.5*asic_to_asic_gap));
	posDL1_3.rotateZ(60.*deg);
	new G4PVPlacement(rotZ300, posDL1_3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	G4ThreeVector posDL2_3; // Using one vector and overwriting it for each part of the hex bugs out, so one vector per side needed
	G4ThreeVector posDL3_3;
	G4ThreeVector posDL4_3;
	G4ThreeVector posDL5_3;
	G4ThreeVector posDL6_3;
	posDL2_3 = posDL1_3;
	posDL2_3.rotateZ(60.*deg);
	posDL3_3 = posDL1_3;
	posDL3_3.rotateZ(120.*deg);
	posDL4_3 = posDL1_3;
	posDL4_3.rotateZ(180.*deg);
	posDL5_3 = posDL1_3;
	posDL5_3.rotateZ(240.*deg);
	posDL6_3 = posDL1_3;
	posDL6_3.rotateZ(300.*deg);
	new G4PVPlacement(rotZ240, posDL2_3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ180, posDL3_3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ120, posDL4_3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ60, posDL5_3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(0, posDL6_3, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);

	// ASIC 5 coverage
	G4ThreeVector posDL1_4 = G4ThreeVector((x_si_start + nSide_w + pSide_w + 0.5*dead_layer_w), 0, (array_mid_Z + 1.5*nSide_d + 1.5*asic_to_asic_gap));
	posDL1_4.rotateZ(60.*deg);
	new G4PVPlacement(rotZ300, posDL1_4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	G4ThreeVector posDL2_4; // Using one vector and overwriting it for each part of the hex bugs out, so one vector per side needed
	G4ThreeVector posDL3_4;
	G4ThreeVector posDL4_4;
	G4ThreeVector posDL5_4;
	G4ThreeVector posDL6_4;
	posDL2_4 = posDL1_4;
	posDL2_4.rotateZ(60.*deg);
	posDL3_4 = posDL1_4;
	posDL3_4.rotateZ(120.*deg);
	posDL4_4 = posDL1_4;
	posDL4_4.rotateZ(180.*deg);
	posDL5_4 = posDL1_4;
	posDL5_4.rotateZ(240.*deg);
	posDL6_4 = posDL1_4;
	posDL6_4.rotateZ(300.*deg);
	new G4PVPlacement(rotZ240, posDL2_4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ180, posDL3_4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ120, posDL4_4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(rotZ60, posDL5_4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);
	new G4PVPlacement(0, posDL6_4, logicDL, "Dead-layer", logicField, false, 0, checkOverlaps);

  	return physWorld;
}	

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials(){

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	world_mat = nist->FindOrBuildMaterial("G4_Galactic");

	// Aluminium
	Al = nist->FindOrBuildMaterial("G4_Al");

    // Silicon
    // Natural -> doping is neglected as 1 part per 100 million will have little effect on nuclear/Monte-Carlo properties
    // Full material definition so can edit later if needed
    G4Element* el_Si = new G4Element("Si", "Si", 14., 28.0855*g/mole); // Args: label, symbol, Z (G4double), avg A (G4double)
    p_Si = new G4Material("p_Si", 2.33*g/cm3, 1);
    p_Si->AddElement(el_Si, 1);
	n_Si = new G4Material("n_Si", 2.33*g/cm3, 1);
    n_Si->AddElement(el_Si, 1);

	// Silicon (DEAD)
    // Natural -> doping is neglected as 1 part per 100 million will have little effect on nuclear/Monte-Carlo properties
    // Full material definition so can edit later if needed
    Dead_Si = new G4Material("Si", 2.33*g/cm3, 1);
    Dead_Si->AddElement(el_Si, 1);

	// Plastic
    Plastic = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

	// Deuterated polyethylene (CD2)
	G4Element* el_C = new G4Element("Carbon", "C", 6., 12.0*g/mole);
    G4Element* el_D = new G4Element("Deuteron", "H", 1., 2.013553212745*g/mole);
    CD2 = new G4Material("CD2", 0.92*g/cm3, 2);
    CD2->AddElement(el_C, 1); // Overloads here. Use ints for nAtoms, or double for mass fractions
    CD2->AddElement(el_D, 2);
}


void DetectorConstruction::DefineMiscGeometry(){}

void DetectorConstruction::ConstructSDandField(){
    // Create uniform magnetic field in the arm logical volume
    G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0,0,fieldStrength)); // magnetic field, uniform, along arm
    G4FieldManager* localFieldMgr = new G4FieldManager(magField);
    localFieldMgr->SetDetectorField(magField);
    localFieldMgr->CreateChordFinder(magField);
    logicField->SetFieldManager(localFieldMgr, true); // args: field manager, (bool) force to all daughters
}
