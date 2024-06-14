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

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"

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

    // Tungsten
    // Natural W for tubes
    G4Material* W = nist->FindOrBuildMaterial("G4_W");

    // Pb-208
    G4Isotope* iso_Pb208 = new G4Isotope("208Lead", 82, 208); // args: name, Z, A, m (default=0)
    G4Element* el_Pb208 = new G4Element("208Lead", "Pb", 1); // args: name, symbol, nIsotopes. This is an overload of G4Element constructor
    el_Pb208->AddIsotope(iso_Pb208, 1.0);
    G4Material* Pb208 = new G4Material("208Lead", 11.35*g/cm3, 1);
    Pb208->AddElement(el_Pb208, 1.);

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

    // Fibreglass (E-glass)
    // G4Materials can be made of other G4Materials so the below compounds are defined like epoxy
    // https://www.sciencedirect.com/topics/chemistry/glass-fiber
    // Missing elements
    G4Element* el_Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
    G4Element* el_Li = new G4Element("Lithium", "Li", 3., 6.941*g/mole);
    G4Element* el_Mg = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
    G4Element* el_Ca = new G4Element("Calcium", "Ca", 20., 40.078*g/mole);
    G4Element* el_Na = new G4Element("Sodium", "Na", 11., 22.989*g/mole);
    G4Element* el_K = new G4Element("Potassium", "K", 19., 39.098*g/mole);
    // SiO2
    G4Material* SiO2 = new G4Material("SiO2", 2.65*g/cm3, 2);
    SiO2->AddElement(el_Si, 1);
    SiO2->AddElement(el_O, 2);
    // Al2O3
    G4Material* Al2O3 = new G4Material("Al2O3", 3.95*g/cm3, 2);
    Al2O3->AddElement(el_Al, 2);
    Al2O3->AddElement(el_O, 3);
    // Li2O
    G4Material* Li2O = new G4Material("Li2O", 2.01*g/cm3, 2);
    Li2O->AddElement(el_Li, 2);
    Li2O->AddElement(el_O, 1);
    // MgO
    G4Material* MgO = new G4Material("MgO", 3.58*g/cm3, 2);
    MgO->AddElement(el_Mg, 1);
    MgO->AddElement(el_O, 1);
    // CaO
    G4Material* CaO = new G4Material("CaO", 3.34*g/cm3, 2);
    CaO->AddElement(el_Ca, 1);
    CaO->AddElement(el_O, 1);
    // Na2O
    G4Material* Na2O = new G4Material("Na2O", 2.27*g/cm3, 2);
    Na2O->AddElement(el_Na, 2);
    Na2O->AddElement(el_O, 1);
    // K2O
    G4Material* K2O = new G4Material("K2O", 2.35*g/cm3, 2);
    K2O->AddElement(el_K, 2);
    K2O->AddElement(el_O, 1);

    // Fibreglass
    G4Material* Fibreglass = new G4Material("Fibreglass", 2.9697*g/cm3, 7); // Overload. Args: label, density, G4int nComponents
    Fibreglass->AddMaterial(SiO2, 0.552);
    Fibreglass->AddMaterial(Al2O3, 0.08);
    Fibreglass->AddMaterial(Li2O, 0.073);
    Fibreglass->AddMaterial(MgO, 0.046);
    Fibreglass->AddMaterial(CaO, 0.187);
    Fibreglass->AddMaterial(Na2O, 0.003);
    Fibreglass->AddMaterial(K2O, 0.002);

    // FR4
    G4Material* FR4 = new G4Material("FR4", 2.242*g/cm3, 2);
    FR4->AddMaterial(Fibreglass, 0.6);
    FR4->AddMaterial(Epoxy, 0.4);

    G4Element* el_N = new G4Element("Nitrogen", "N", 7., 14.0067*g/mole);
    G4Material* arm_vacuum = new G4Material("vacuum", 0.0000000001*g/cm3, 2);
    arm_vacuum->AddElement(el_N, 0.75);
    arm_vacuum->AddElement(el_O, 0.25);


    // ############
    // Parameters
    // ############
    // Change these variables to change key parameters of the geometry. Geant uses half-lengths most of the time.
    // Z axis aligns the source and detector.

    G4double worldXY = 50*cm;
    G4double worldZ = 50*cm;

    // Target
    G4double rTarget = 1*cm;
    G4double zHalfTarget = 4.4053E-7*m; // 1 mg/cm2 of Pb

    // #### End of beam tube to target distance ####
    G4double x = 1.*mm;

    G4ThreeVector posTarget;
    posTarget.setZ(-x-zHalfTarget);

    // Beam tube
    G4double rBTInner = 2.*cm;
    G4double rBTOuter = 3.*cm;
    G4double zHalfBT = 5.*cm;
    G4ThreeVector posBT;
    posBT.setZ(-zHalfBT);

    // Arm 1
    G4double rArmInner = 2.*cm;
    G4double rArmOuter = 3.*cm;
    G4double zHalfArm = 15.*cm;
    G4ThreeVector posArm;
    posArm.setX(rBTOuter+zHalfArm);
    posArm.setZ(rArmInner);
    G4RotationMatrix* rotArm = new G4RotationMatrix();
    rotArm->rotateY(90.*deg);

    // DSSD
    // Plastic
    G4double xHalfInner = 3.5*cm;
    G4double xHalfOuter = 5.5*cm;
    G4double large = 10*cm; // for boolean operations
    G4double zHalfPlastic = 0.5*mm;
    G4ThreeVector posPlastic1, posPlastic2, posFR4;

    // FR4
    G4double zHalfFR4 = 2.5*mm;
    posPlastic1.setX(rBTOuter+(2*zHalfArm)+zHalfPlastic+3.*mm);
    posPlastic1.setZ(rArmInner);
    posPlastic2.setX(rBTOuter+(2*zHalfArm)+(3*zHalfPlastic)+(2*zHalfFR4)+3.*mm);
    posPlastic2.setZ(rArmInner);
    posFR4.setX(rBTOuter+(2*zHalfArm)+(2*zHalfPlastic)+zHalfFR4+3.*mm);
    posFR4.setZ(rArmInner);

    // Pixels
    G4double xHalfPxl = 0.5*mm;
    G4double zHalfPxl = 0.25*mm;

    G4String label;
    G4ThreeVector posPxl = G4ThreeVector((rBTOuter+(2*zHalfArm)+(2*zHalfPlastic)+(2*zHalfFR4)+zHalfPxl+3.*mm), -34.5*mm, rArmInner-34.5*mm);

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

    // -------
    // Target
    // -------
    G4Tubs* solidTarget = new G4Tubs("Target", 0., rTarget, zHalfTarget, 0, 360.*deg); // args: name, R in, R out, half length Z, ang start, ang range
    G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, Pb208, "Target"); // args: solid volume, material, name
    new G4PVPlacement(0, posTarget, logicTarget, "Target", logicWorld, 0, 0, true); // args: rotation, translation, logic volume, name, parent volume, ignore, copy num, fCheckOverlaps

    // ---------
    // Beamtube
    // ---------
    G4Tubs* solidBT = new G4Tubs("Beamtube", rBTInner, rBTOuter, zHalfBT, 0, 360.*deg);
    G4LogicalVolume* logicBT = new G4LogicalVolume(solidBT, W, "Beamtube");
    new G4PVPlacement(0, posBT, logicBT, "Beamtube", logicWorld, 0, 0, true);

    // ----
    // Arm
    // ----
    G4Tubs* solidArm = new G4Tubs("Arm", rArmInner, rArmOuter, zHalfArm, 0, 360.*deg);
    G4LogicalVolume* logicArm = new G4LogicalVolume(solidArm, W, "Arm");
    new G4PVPlacement(rotArm, posArm, logicArm, "Arm", logicWorld, 0, 0, true);

    // -----------
    // Arm (space)
    // -----------
    G4Tubs* solidArmSpace = new G4Tubs("ArmSpace", 0., rArmInner-0.00001*mm, zHalfArm, 0, 360.*deg);
    logicArmSpace = new G4LogicalVolume(solidArmSpace, arm_vacuum, "ArmSpace");
    new G4PVPlacement(rotArm, posArm, logicArmSpace, "ArmSpace", logicWorld, 0, 0, true);

    // -----
    // DSSD
    // -----
    // CASE
    // Plastic cover
    G4Box* solidPlasticBox1 = new G4Box("Plastic1", xHalfOuter, xHalfOuter, zHalfPlastic); // args: name, x, y, z half lengths
    G4Box* solidPlasticBox2 = new G4Box("Plastic2", xHalfInner, xHalfInner, zHalfPlastic+1.*mm);
    // Box1 - Box2 = final shape
    G4SubtractionSolid* solidPlastic = new G4SubtractionSolid("Plastic", solidPlasticBox1, solidPlasticBox2, 0, G4ThreeVector()); // args: name, obj1, obj2, rotation of 2 w.r.t 1, translation of 2 w.r.t 1
    G4LogicalVolume* logicPlastic = new G4LogicalVolume(solidPlastic, Plastic, "Plastic");
    new G4PVPlacement(rotArm, posPlastic1, logicPlastic, "Plastic", logicWorld, 0, 0, true);
    new G4PVPlacement(rotArm, posPlastic2, logicPlastic, "Plastic", logicWorld, 0, 0, true);
    
    // FR4 board
    G4Box* solidFR4Box1 = new G4Box("FR4-1", xHalfOuter, xHalfOuter, zHalfFR4);
    G4Box* solidFR4Box2 = new G4Box("FR4-2", xHalfInner, xHalfInner, zHalfFR4+1.*mm);
    G4SubtractionSolid* solidFR4 = new G4SubtractionSolid("FR4", solidFR4Box1, solidFR4Box2, 0, G4ThreeVector());
    G4LogicalVolume* logicFR4 = new G4LogicalVolume(solidFR4, FR4, "FR4");
    new G4PVPlacement(rotArm, posFR4, logicFR4, "FR4", logicWorld, 0, 0, true);

    // PIXELS
    G4Box* solidPxl = new G4Box("Pxl", xHalfPxl, xHalfPxl, zHalfPxl);
    G4LogicalVolume* logicPxl = new G4LogicalVolume(solidPxl, Si, "Silicon");

    // place pixels using nested for loops
    fSiliconPV = new G4PVPlacement(rotArm, posPxl, logicPxl, "Si", logicWorld, 0, 0, true); // place 1st pxl manually so fSiliconPV pointer can have a value
    for(int i=0; i<70; i++){ // y iteration
        for(int j=0; j<70; j++){ // x iteration
            label = "Si-"+G4String(std::to_string(j))+ "-" + G4String(std::to_string(i));
            G4ThreeVector movement = G4ThreeVector(0, i*mm, j*mm);
            if(i != 0 || j != 0){ // don't repeat (0,0)
                new G4PVPlacement(rotArm, (posPxl+movement), logicPxl, "Si", logicWorld, 0, (70*i +j), true);
            }
        }
    }

    return physWorld;
}

void OurDetectorConstruction::ConstructSDandField(){
    // Create uniform magnetic field in the arm logical volume
    G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0.05*tesla,0.,0.)); // magnetic field, uniform, along arm
    G4FieldManager* localFieldMgr = new G4FieldManager(magField);
    localFieldMgr->SetDetectorField(magField);
    localFieldMgr->CreateChordFinder(magField);
    logicArmSpace->SetFieldManager(localFieldMgr, true); // args: field manager, (bool) force to all daughters
}