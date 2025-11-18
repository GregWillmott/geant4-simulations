// Grab our corresponding header for this class
#include "../include/detectorConstruction.hh"

// G4 libraries needed
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh" // cylinder
#include "G4UnionSolid.hh" // boolean solid
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* OurDetectorConstruction::Construct() // Function from Geant that will be recognised at compilation
{
    // ----------------------
    // Materials
    // ----------------------
    G4NistManager* nist = G4NistManager::Instance(); // NIST materials database. All materials are in appendix of Book For Application Developers pg 420
    // Alternative is to build materials manually from either atomic or isotopic level.

    // World material = air
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    // Detector case = natural aluminium
    G4double z, a, density;
    G4Material* natAl = new G4Material("Aluminium", z=13., a=26.9816*g/mole, density=2.7*g/cm3);

    // Crystal = natural Germanium
    G4Material* natGe = new G4Material("Germanium", z=32, a=72.6*g/mole, density=5.32*g/cm3);

    // --------------------
    // Parameters
    // --------------------
    // Change these variables to change key parameters of the geometry. Geant uses half-lengths most of the time.
    // Z axis aligns the source and detector.
    G4double worldXY = 30/2*cm;
    G4double worldZ = 100/2*cm;
    
    G4double GeOuterRad = 5*cm;
    G4double GeCylZ = 5*cm;
    
    G4double AlOuterRad = 6*cm;
    G4double AlCylZ = 5.5*cm;
    
    G4double AlEndZ = 0.5*cm;
    
    G4ThreeVector AlCentre = G4ThreeVector(0., 0., 24.5*cm); // Al outer tube casing
    G4ThreeVector GeCentre = G4ThreeVector(0., 0., 25.*cm); // Ge cylinder centre

    // ----------
    // World
    // ----------
    G4Box* solidWorld = new G4Box("World", worldXY, worldXY, worldZ); // Args: name, x 1/2 length, y 1/2 length, z 1/2 length.
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World"); // Args: solid volume (shape), material, name.
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, 0, 0, true); // New physical volume placement:
    // Args: rotation vector, translation vector, logical volume being placed, name, mother volume, boolean operations, copy number, overlap checking.
    
    // -------------------
    // Germanium cylinder
    // -------------------
    G4Tubs* solidGe = new G4Tubs("Ge-cyl", 0., GeOuterRad, GeCylZ, 0, 360*deg); // Args: name, inner rad, outer rad, Z 1/2 length, start phi angle, phi angle covered.
    G4LogicalVolume* logicGe = new G4LogicalVolume(solidGe, natGe, "Ge-cyl");
    // physWorld is only made once, daughter volumes are placed by just using the PVPlacement function. Here however, we need a pointer to refer to this
    // physical volume so the line has a left-hand-side. Look in header for more detail.
    fGermaniumPV = new G4PVPlacement(0, GeCentre, logicGe, "Ge-cyl", logicWorld, 0, 0, true);
    // logical volume of the world is what's specified as the mother volume, not physical volume physWorld.

    // -------------------
    // Aluminium cylinder
    // -------------------
    // Must be a boolean solid: union of tube surrounding the Ge, and the disc end cap in front of the Ge.
    G4Tubs* solidAlTube = new G4Tubs("Al-tube", GeOuterRad, AlOuterRad, AlCylZ, 0, 360*deg);
    G4Tubs* solidAlEnd = new G4Tubs("Al-disc", 0., GeOuterRad, AlEndZ, 0, 360*deg);
    G4VSolid* solidAl = new G4UnionSolid("Al-cyl", solidAlTube, solidAlEnd, nullptr, G4ThreeVector(0., 0., -(AlCylZ - AlEndZ)));

    G4LogicalVolume* logicAl = new G4LogicalVolume(solidAl, natAl, "Al-cyl");
    
    new G4PVPlacement(0, AlCentre, logicAl, "Al-cyl", logicWorld, 0, 0, true);

    return physWorld;
}