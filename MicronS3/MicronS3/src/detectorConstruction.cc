// Grab our corresponding header for this class
#include "../include/detectorConstruction.hh"

// G4 libraries needed
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh" // cylinder
// boolean solids
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"

#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <iostream>

G4VPhysicalVolume* OurDetectorConstruction::Construct() // Function from Geant that will be recognised at compilation
{
    // ###########
    // Materials
    // ###########
    G4NistManager* nist = G4NistManager::Instance(); // NIST materials database. All materials are in appendix of Book For Application Developers pg 420
    // Alternative is to build materials manually from either atomic or isotopic level.

    // World material
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");

    // Silicon
    // Natural -> doping is neglected as 1 part per 100 million will have little effect on nuclear/Monte-Carlo properties
    // Full material definition so can edit later if needed
    G4Element* el_Si = new G4Element("Si", "Si", 14., 28.0855*g/mole); // Args: label, symbol, Z (G4double), avg A (G4double)
    G4Material* Si = new G4Material("Si", 2.33*g/cm3, 1);
    Si->AddElement(el_Si, 1);

    // Covers for FR4 circuit board
    // Use NIST default
    G4Material* Plastic = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

    // Titanium
    // Natural Ti for normalisation target
    G4Material* Ti = nist->FindOrBuildMaterial("G4_Ti");

    // 120Sn
    // Assume pure 120 Tin for now
    G4Isotope* Sn120 = new G4Isotope("120Sn", 50, 120); // Args: label, Z, N, G4double a (defPlasticlt 0.), G4int m (defPlasticlt 0)
    // Think the API is wrong. Arg 3 in the previous line is probably A, not N.
    G4Element* el_Sn = new G4Element("120Sn", "Sn", 1); // Overload to normal definition. Args: label, symbol, G4int nbIsotopes
    el_Sn->AddIsotope(Sn120, 1);
    G4Material* Sn = new G4Material("120Sn", 7.31*g/cm3, 1);
    Sn->AddElement(el_Sn, 1);
    G4cout << "test" << G4endl;

    // FR4
    // Fire Retardant 4 circuit board. 60% Fibreglass, 40% epoxy resin
    // Fibreglass: Si02 : 0.552, Al2O3 : 0.148, B2O3 : 0.073, MgO : 0.033, CaO : 0.187 + trace compounds
    // Epoxy resin : C21H25ClO5
    // Epoxy
    G4Element* el_C = new G4Element("Carbon", "C", 6., 12.011*g/mole);
    G4Element* el_H = new G4Element("Hydrogen", "H", 1., 1.00784*g/mole);
    G4Element* el_Cl = new G4Element("Chlorine", "Cl", 17., 35.453*g/mole);
    G4Element* el_O = new G4Element("Oxygen", "O", 8., 15.999*g/mole);
    G4Material* Epoxy = new G4Material("Epoxy-resin", 1.15*g/cm3, 4);
    Epoxy->AddElement(el_C, 21); // Overloads here. Use ints for nAtoms, or double for mass fractions
    Epoxy->AddElement(el_H, 25);
    Epoxy->AddElement(el_Cl, 1);
    Epoxy->AddElement(el_O, 5);

    // Fibreglass
    // G4Materials can be made of other G4Materials so the below compounds are defined like epoxy
    // Missing elements
    G4Element* el_Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
    G4Element* el_B = new G4Element("Boron", "B", 5., 10.811*g/mole);
    G4Element* el_Mg = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
    G4Element* el_Ca = new G4Element("Calcium", "Ca", 20., 40.078*g/mole);
    // SiO2
    G4Material* SiO2 = new G4Material("SiO2", 2.65*g/cm3, 2);
    SiO2->AddElement(el_Si, 1);
    SiO2->AddElement(el_O, 2);
    // Al2O3
    G4Material* Al2O3 = new G4Material("Al2O3", 3.95*g/cm3, 2);
    Al2O3->AddElement(el_Al, 2);
    Al2O3->AddElement(el_O, 3);
    // B2O3
    G4Material* B2O3 = new G4Material("B2O3", 2.46*g/cm3, 2);
    B2O3->AddElement(el_B, 2);
    B2O3->AddElement(el_O, 3);
    // MgO
    G4Material* MgO = new G4Material("MgO", 3.58*g/cm3, 2);
    MgO->AddElement(el_Mg, 1);
    MgO->AddElement(el_O, 1);
    // CaO
    G4Material* CaO = new G4Material("CaO", 3.34*g/cm3, 2);
    CaO->AddElement(el_Ca, 1);
    CaO->AddElement(el_O, 1);
    // Fibreglass
    G4Material* Fibreglass = new G4Material("Fibreglass", 2.9697*g/cm3, 5); // Overload. Args: label, density, G4int nComponents
    Fibreglass->AddMaterial(SiO2, 0.552);
    Fibreglass->AddMaterial(Al2O3, 0.148);
    Fibreglass->AddMaterial(B2O3, 0.073);
    Fibreglass->AddMaterial(MgO, 0.033);
    Fibreglass->AddMaterial(CaO, 0.187);
    // FR4
    G4Material* FR4 = new G4Material("FR4", 2.242*g/cm3, 2);
    FR4->AddMaterial(Fibreglass, 0.6);
    FR4->AddMaterial(Epoxy, 0.4);

    // Lead
    G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");


    // ############
    // Parameters
    // ############
    // Change these variables to change key parameters of the geometry. Geant uses half-lengths most of the time.
    // Z axis aligns the source and detector.

    G4double worldXY = 12*cm;
    G4double worldZ = 25*cm;
    
    G4double detFrontFaceZ = 5.2*cm;

    // Si strips
    // 24 rings (25 bounds)
    G4double radii[] = {11.*mm, 12.*mm, 13.*mm, 14.*mm, 15.*mm, 16.*mm, 17.*mm, 18.*mm, 19.*mm, 20.*mm, 21.*mm, 22.*mm, 23.*mm,
                        24.*mm, 25.*mm, 26.*mm, 27.*mm, 28.*mm, 29.*mm, 30.*mm, 31.*mm, 32.*mm, 33.*mm, 34.*mm, 35.*mm};
    G4double phiSegment = 11.25*deg;
    // 32 sects
    G4double phis[] = {354.375*deg, 5.625*deg, 16.875*deg, 28.125*deg, 39.375*deg, 50.625*deg, 61.875*deg, 73.125*deg, 84.375*deg, 95.625*deg, 106.875*deg, 
                        118.125*deg, 129.375*deg, 140.625*deg, 151.875*deg, 163.125*deg, 174.375*deg, 185.625*deg, 196.875*deg, 208.125*deg, 219.375*deg,
                        230.625*deg, 241.875*deg, 253.125*deg, 264.375*deg, 275.625*deg, 286.875*deg, 298.125*deg, 309.375*deg, 320.625*deg, 331.875*deg,
                        343.125*deg};
    G4double halfZSi = 0.05*mm;
    // FR4 board
    G4double halfZFR4 = 1.15*mm;
    G4double halfShortSideFR4 = 60.*mm;
    G4double halfLongSideFR4 = 70.711*mm;
    G4double halfZPlastic = 0.025*mm;
    G4double rLarge = 10.*cm;

    G4RotationMatrix* pRot = new G4RotationMatrix(); // rotation is expected as a pointer by a lot of G4 functions
    pRot->rotateZ(45.*deg);

    G4RotationMatrix* pPhiRot;
    G4String label;
    G4Tubs* sRingj;
    G4LogicalVolume* lRingj;

    // ######################
    bool if_Ti_target = true;
    // ######################

    // Ti target
    G4double halfXYTarget = 1.*cm;
    G4double halfZTi = 0.00111*mm;
    // G4double halfZTi = 0.01*mm;

    // 120Sn target
    // G4double halfZSn = 0.000958*mm; // 0.5 mg/cm2
    G4double halfZSn = 0.00222*mm; // 1 mg/cm2
    // G4double halfZSn = 0.01*mm;
    G4double targetXRot = -22.5*deg; // Check this
    G4RotationMatrix* pTargetRot = new G4RotationMatrix();
    pTargetRot->rotateX(targetXRot);

    // Beam Stop
    G4double halfXYStop = 2.5*cm;
    G4double halfZStop = 2.5*cm;
    G4ThreeVector posStop = G4ThreeVector(0.,0.,13.*cm);

    // ####################
    // Volume construction
    // ####################

    // ----------
    // World
    // ----------
    G4Box* solidWorld = new G4Box("World", worldXY, worldXY, worldZ); // Args: name, x 1/2 length, y 1/2 length, z 1/2 length.
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World"); // Args: solid volume (shape), material, name.
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, 0, 0, true); // New physical volume placement:
    // Args: rotation vector, translation vector, logical volume being placed, name, mother volume, boolean operations?, copy number, overlap checking.
    
    // ---------
    // Board
    // ---------
    // The shape is the intersection of two squares, one larger and offset by 45 deg. This is then intersected with a hollow cylinder to cut the space
    // in the middle for the Si
    G4Box* FR4ShortBox = new G4Box("FR4-short-box", halfShortSideFR4, halfShortSideFR4, halfZFR4);
    G4Box* FR4LongBox = new G4Box("FR4-long-box", halfLongSideFR4, halfLongSideFR4, halfZFR4);
    G4IntersectionSolid* FR4CutBox = new G4IntersectionSolid("FR4-cut-box", FR4ShortBox, FR4LongBox, pRot, G4ThreeVector()); // Args:
    // label, original solid vol, moving solid vol, rotation, translation
    G4Tubs* FR4Hole = new G4Tubs("FR4-hole", radii[24], rLarge, 5.*cm, 0, 360.*deg); // Args: label, rMin, rMax, half-depth, start ang, ang range 
    G4IntersectionSolid* sFR4Board = new G4IntersectionSolid("FR4", FR4CutBox, FR4Hole, 0, G4ThreeVector()); // Args: label, vol 1, vol 2, rotation, translation applied to vol 2

    G4LogicalVolume* lFR4Board = new G4LogicalVolume(sFR4Board, FR4, "FR4");
    new G4PVPlacement(0, G4ThreeVector(0, 0, detFrontFaceZ+halfZFR4+(2*halfZPlastic)), lFR4Board, "FR4", logicWorld, 0, 0, true);

    // Reuse cutbox and a very small Z cylinder so make a thinner version of FR4Board
    G4Tubs* PlasticHole = new G4Tubs("Plastic-hole", radii[24], rLarge, halfZPlastic, 0, 360.*deg);
    G4IntersectionSolid* sPlasticBoard = new G4IntersectionSolid("Plastic", FR4CutBox, PlasticHole, 0, G4ThreeVector());

    G4LogicalVolume* lPlasticBoard = new G4LogicalVolume(sPlasticBoard, Plastic, "Plastic");
    // Upstream Plastic
    new G4PVPlacement(0, G4ThreeVector(0, 0, detFrontFaceZ+halfZPlastic), lPlasticBoard, "Plastic", logicWorld, 0, 0, true);
    // Downstream Plastic
    new G4PVPlacement(0, G4ThreeVector(0, 0, detFrontFaceZ+(3*halfZPlastic)+(2*halfZFR4)), lPlasticBoard, "Plastic", logicWorld, 0, 0, true);

    // ---------
    // Silicon
    // ---------
    // Place first segment
    G4Tubs* sRing1 = new G4Tubs("SiRing1", radii[0], radii[1], halfZSi, 354.375*deg, 11.25*deg); // Args: label, rMin, rMax, half depth, start ang, ang range
    G4LogicalVolume* lRing1 = new G4LogicalVolume(sRing1, Si, "SiRing1");
    fSiliconPV = new G4PVPlacement(0, G4ThreeVector(0, 0, detFrontFaceZ+halfZSi), lRing1, "Silicon-0-0", logicWorld, 0, G4int(0), true);
    
    // Iterate along ring number
    for(int j = 0; j < 24; j++){
        // Create solid volume using cpp vector of radii
        sRingj = new G4Tubs("SiRingj", radii[j], radii[j+1], halfZSi, 354.375*deg, 11.25*deg);
        // Logical volume
        lRingj = new G4LogicalVolume(sRingj, Si, "SiRingj");

        // Place sectors of a given ring number
        for(int i = 0; i < 32; i++){
            if((i >= 1) || (j >= 1)){ // 1st already placed
                pPhiRot = new G4RotationMatrix();
                pPhiRot->rotateZ((i*11.25)*deg);
                label = "Silicon-"+G4String(std::to_string(j))+ "-" + G4String(std::to_string(i));
                new G4PVPlacement(pPhiRot, G4ThreeVector(0, 0, detFrontFaceZ+halfZSi), lRingj, label, logicWorld, 0, G4int(32*j + i), true);
                // index formula for copy number goes 0 -> 23 for ring 1, 24 -> 47 for ring 2 etc
            }
        }
    }

    // -------
    // Target
    // -------
    if(if_Ti_target){
        // Titanium
        G4Box* sTiBox = new G4Box("Ti-target", halfXYTarget, halfXYTarget, halfZTi);
        G4LogicalVolume* lTiBox = new G4LogicalVolume(sTiBox, Ti, "Ti");
        new G4PVPlacement(pTargetRot, G4ThreeVector(), lTiBox, "Ti", logicWorld, 0, 0, true);
    }
    else{
        // 120-Sn
        G4Box* sSnBox = new G4Box("Sn-target", halfXYTarget, halfXYTarget, halfZSn);
        G4LogicalVolume* lSnBox = new G4LogicalVolume(sSnBox, Sn, "Sn");
        new G4PVPlacement(pTargetRot, G4ThreeVector(), lSnBox, "Sn", logicWorld, 0, 0, true);
    }

    // ----------
    // Beam stop
    // ----------
    G4Box* sBeamStop = new G4Box("BeamStop", halfXYStop, halfXYStop, halfZStop);
    G4LogicalVolume* lBeamStop = new G4LogicalVolume(sBeamStop, Pb, "BeamStop");
    fLeadPV = new G4PVPlacement(0, posStop, lBeamStop, "BeamStop", logicWorld, 0, 0, true);

    return physWorld;
}