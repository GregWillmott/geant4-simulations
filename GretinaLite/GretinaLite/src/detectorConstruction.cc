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
    // G4Material* natAl = new G4Material("Aluminium", z=13., a=26.9816*g/mole, density=2.7*g/cm3);
    G4Material* natAl = nist->FindOrBuildMaterial("G4_Al");

    // Crystal = natural Germanium
    // G4Material* natGe = new G4Material("Germanium", z=32, a=72.6*g/mole, density=5.32*g/cm3);
    G4Material* natGe = nist->FindOrBuildMaterial("G4_Ge");

    // Contacts = natural copper
    // G4Material* natCu = new G4Material("Copper", z=29, a=63.546*g/mole, density=8.96*g/cm3);
    G4Material* natCu = nist->FindOrBuildMaterial("G4_Cu");




    // --------------------
    // Parameters
    // --------------------
    // Change these variables to change key parameters of the geometry. Geant uses half-lengths most of the time.
    // Z axis aligns the source and detector.

    G4double worldXY = 150*cm;
    G4double worldZ = 250*cm;
    
    // Aluminium Gretina shell
    G4double shellRMin = (1022./2.)*mm;
    G4double shellRMax = (1276./2.)*mm;
    // Port holes
    G4double smallPortR = 8.89*cm;
    G4double modulePortR = 15.25016*cm;

    // Variables for Germanium hexes. Most of these are for G4Polyhedra
    G4int nSides = 6;
    G4double phiMax = 360.*deg;
    G4int nZPlanes = 2;
    G4double locZPlanes[] = {0.*mm, 90.*mm}; // See Paschalis diagram
    G4double rOuter[] = {19.052*mm, 44.106*mm}; // I calculated these from drawing equilateral and right angle triangles.
    G4double rInner[] = {0.,0.};
    G4RotationMatrix* hexRot30 = new G4RotationMatrix();
    hexRot30->rotateX(0.);
    hexRot30->rotateY(0.);
    hexRot30->rotateZ(30.*deg);

    // Germanium cylinder
    G4double GeCylROuter = 40.*mm;
    G4double GeCylZ = 90.*2.*mm; // Double as it goes along +,- Z. The hex is only +Z
    G4String GeName = "GeCylHex"; // Ran into function which only accepts G4String inputs, can't typecast where needed

    // Coaxial cut-outs in the Ge cylinder
    G4double rCoax = 5.*mm;
    G4double coaxCylZ = (71./2.)*mm; // This is a half-length
    G4ThreeVector coaxSphCen = G4ThreeVector(0, 0, 20.*mm);
    G4ThreeVector coaxCylCen = G4ThreeVector(0, 0, 35.51*mm); // 35 for cyl half Z

    // Vectors & Rotations for stacking crystals together in a clover
    // Indexing looking from the front: left - 1, right - 2, up - 3, down - 4. 0 is midpoint on the face
    // 0 -> 1
    G4ThreeVector transl0to1 = G4ThreeVector(18.541*mm, 0., -(4.383)*mm);
    G4RotationMatrix rot0to1 = G4RotationMatrix();
    rot0to1.rotateX(0);
    rot0to1.rotateY((13.325)*deg);
    rot0to1.rotateZ(0);

    // G4MultiUnion accepts these as argument rather than rot and transl separately
    G4Transform3D transf0to1 = G4Transform3D(rot0to1,transl0to1);

    // 0 -> 2
    G4ThreeVector transl0to2 = G4ThreeVector(-18.541*mm, 0., -(4.383)*mm);
    G4RotationMatrix rot0to2 = G4RotationMatrix();
    rot0to2.rotateX(0);
    rot0to2.rotateY(-(13.325)*deg);
    rot0to2.rotateZ(0);
    G4Transform3D transf0to2 = G4Transform3D(rot0to2, transl0to2);

    // 0 -> 3
    G4ThreeVector transl0to3 = G4ThreeVector();
    transl0to3 = G4ThreeVector(0, 32.*mm, -9.5*mm);
    G4RotationMatrix rot0to3 = G4RotationMatrix();
    rot0to3.rotateX(-(26.)*deg);
    rot0to3.rotateY(0);
    rot0to3.rotateZ(0);
    G4Transform3D transf0to3 = G4Transform3D(rot0to3, transl0to3);

    // 0 -> 4
    G4ThreeVector transl0to4 = G4ThreeVector();
    transl0to4 = G4ThreeVector(0, -32.*mm, -9.5*mm);
    G4RotationMatrix rot0to4 = G4RotationMatrix();
    rot0to4.rotateX((26.)*deg);
    rot0to4.rotateY(0);
    rot0to4.rotateZ(0);
    G4Transform3D transf0to4 = G4Transform3D(rot0to4, transl0to4);

    // Aluminium chamber and beam lines
    G4double chamberRMin = 178.5*mm;
    G4double chamberRMax = 180.*mm;
    // BT = Beam tube
    G4double BTRMin = 18.5*mm;
    G4double BTRMax = 20.*mm;
    G4double BTZLen = (5.*cm + (127.6/2.*cm) - BTRMax) /2.; // From UCGretina + 5cm
    G4ThreeVector BTLocVector = G4ThreeVector(0., 0., (sqrt(pow(chamberRMin, 2.)-pow(BTRMax, 2.)) + BTZLen - 5.*cm)); 
    // ~ 5cm overhang into chamber isn't in UCGretina but is in pictures.
    // BT cones
    G4double BTConsRMin1 = 18.5*mm;
    G4double BTConsRMax1 = 20.*mm;
    G4double BTConsRMin2 = 0.9*smallPortR - 2.5*mm;
    G4double BTConsRMax2 = 0.9*smallPortR;
    G4double BTConsZ = ((shellRMin - chamberRMin) /2.) - 2.5*cm;
    G4ThreeVector BTConsLocVector = G4ThreeVector(0., 0., 1.5*cm - (shellRMax - shellRMin)); // Extra gap given to stop overlap with chamber
    G4RotationMatrix* BTConeFlip = new G4RotationMatrix();
    BTConeFlip->rotateY(180.*deg);

    // Spherical subtraction in the front face of the Ge crystals (borrows from Al shell)
    G4ThreeVector subSphLoc = G4ThreeVector(0, 0, -chamberRMin);

    // Copper contacts on the back of the Ge crystals
    G4double cuContactZ = 10.*mm; // 20mm total. Use GeCylROuter for radius
    // Contact 1
    G4ThreeVector contact1Loc = G4ThreeVector(41.55*mm, 0, 92.94*mm);
    G4Transform3D transfContact1 = G4Transform3D(rot0to1, contact1Loc);
    // Contact 2
    G4Transform3D transfContact2 = G4Transform3D(rot0to2, G4ThreeVector(-41.55*mm, 0, 92.94*mm));
    // Contact 3
    G4Transform3D transfContact3 = G4Transform3D(rot0to3, G4ThreeVector(0, 75.79*mm, 80.426*mm));
    // Contact 4
    G4Transform3D transfContact4 = G4Transform3D(rot0to4, G4ThreeVector(0, -75.79*mm, 80.426*mm));

    // Homogenised Aluminium module back end

    // Represent FET compartment as the union of two cuboids, one rotated about Z by 45 degrees
    G4double fetSquLen1x = 7.5*cm;
    G4double fetSquLen1y = 11.*cm; // Already halved
    G4double fetSquLen2 = 9.5*cm;
    G4double fetZ = (10.16/2.)*cm;
    G4ThreeVector fetLoc = G4ThreeVector(0, 0, (11.5*cm + fetZ));
    G4RotationMatrix* z45Rot = new G4RotationMatrix();
    z45Rot->rotateZ(45.*deg); 
    // Preamp compartment
    G4double preampR = 12.7*cm;
    G4double preampZ = (17.78/2)*cm;
    G4ThreeVector preampLoc = fetLoc + G4ThreeVector(0, 0, fetZ) + G4ThreeVector(0, 0, preampZ);
    // Let section after crystals, FET, Preamp be section 4
    G4double sec4R = 15.25*cm;
    G4double sec4Z = (10.16/2.)*cm;
    G4ThreeVector sec4Loc = preampLoc + G4ThreeVector(0, 0, preampZ) + G4ThreeVector(0, 0, sec4Z);
    // Stopper in section 4 that rests against the Gretina shell
    G4double stopperR = 20.32*cm; // 8.'
    G4double stopperZ = (2.54/2.)*cm; // 0.5'
    G4ThreeVector stopperLoc = sec4Loc + G4ThreeVector(0, 0, 2.54/2.*cm);
    // Section after 4 is section 5
    G4double sec5R = 10.16*cm;
    G4double sec5Z = (10.16/2.)*cm;
    G4ThreeVector sec5Loc = sec4Loc + G4ThreeVector(0, 0, sec4Z) + G4ThreeVector(0, 0, sec5Z);
    // Dewar
    G4double dewarR = 12.7*cm;
    G4double dewarZ = (35.56/2.)*cm;
    G4ThreeVector dewarLoc = sec5Loc + G4ThreeVector(0, 0, sec5Z) + G4ThreeVector(0, 0, dewarZ);

    // Aluminium Gretina shell
    G4ThreeVector Pos0 = G4ThreeVector(0, 0, 180.*mm);
    G4RotationMatrix shellRot0 = G4RotationMatrix::IDENTITY;
    // Orients the cluster before being placed according to Euler angles - UCGretina
    // shellRot0.rotateY(Pos0.getTheta());
    // shellRot0.rotateZ(Pos0.getPhi());

    // Portholes in the shell
    G4ThreeVector locPort;
    G4ThreeVector locModule;
    G4RotationMatrix rotPort;
    G4RotationMatrix rotModule;
    G4int nModules = 13;

    // Indicies are port number, then axis. Rotations from UCGretina
    // Ports are from North hemisphere and correspond to slots 0, 1, 5, 6, 10, 11, 12, 13, 14, 20, 21, 25, 26
    G4double portRotations [nModules][3];
    portRotations[0][0] = 0.;
    portRotations[0][1] = 31.717471*deg;
    portRotations[0][2] = -54.*deg;
    portRotations[1][0] = 0.;
    portRotations[1][1] = 31.717471*deg;
    portRotations[1][2] = 18.*deg;
    portRotations[2][0] = 90.*deg;
    portRotations[2][1] = 58.282526*deg;
    portRotations[2][2] = -18.*deg;
    portRotations[3][0] = 90.*deg;
    portRotations[3][1] = 58.282526*deg;
    portRotations[3][2] = 54.*deg;
    portRotations[4][0] = -31.75*deg;
    portRotations[4][1] = 90.*deg;
    portRotations[4][2] = -72.*deg;
    portRotations[5][0] = -31.75*deg;
    portRotations[5][1] = 90.*deg;
    portRotations[5][2] = -36.*deg;
    portRotations[6][0] = -31.75*deg;
    portRotations[6][1] = 90.*deg;
    portRotations[6][2] = 0.*deg;
    portRotations[7][0] = 31.75*deg;
    portRotations[7][1] = 90.*deg;
    portRotations[7][2] = 36.*deg;
    portRotations[8][0] = -31.75*deg;
    portRotations[8][1] = 90.*deg;
    portRotations[8][2] = 72.*deg;
    portRotations[9][0] = -90.*deg;
    portRotations[9][1] = 121.717475*deg;
    portRotations[9][2] = -54.*deg;
    portRotations[10][0] = -90.*deg;
    portRotations[10][1] = 121.717475*deg;
    portRotations[10][2] = 18.*deg;
    portRotations[11][0] = 180.*deg;
    portRotations[11][1] = 148.282526*deg;
    portRotations[11][2] = -18.*deg;
    portRotations[12][0] = 180.*deg;
    portRotations[12][1] = 148.282526*deg;
    portRotations[12][2] = 54.*deg;

    // Transformation objects (vector of G4Transform3D's)
    G4Transform3D transfModuleVec [nModules]; // for modules
    G4Transform3D transfPortVec [nModules]; // for cutting holes in the shell

    // G4double port0Rotations [3] = {0., 31.717471*deg, -54.*deg}; // From UCGretina

    // Loop over all modules to be placed
    for(int i = 0; i < nModules; i++){
        // ---- Port transformation ----
        
        // Reset obj
        rotPort = G4RotationMatrix();
        // Put in rotations
        rotPort.rotateZ(portRotations[i][0]);
        rotPort.rotateY(portRotations[i][1]);
        rotPort.rotateZ(portRotations[i][2]);

        // Make use of spherical geometry and use spherical coords to find where the port should go
        // G4ThreeVector docs are instead listed as Hep3Vector as it's just an alias
        // Reset obj
        locPort = G4ThreeVector(0., 0., 180.*mm);
        locPort.setMag((shellRMin + shellRMax)/2.);
        // Put in rotations
        locPort.rotateZ(portRotations[i][0]);
        locPort.rotateY(portRotations[i][1]);
        locPort.rotateZ(portRotations[i][2]);

        // Put port transform info into vector storage object
        transfPortVec[i] = G4Transform3D(rotPort, locPort);


        // ---- Module transformation ---

        // Reset objs
        rotModule = rotPort;

        // locModule = G4ThreeVector(0, 0, 180.*mm);
        locModule = locPort;
        locModule.setMag(chamberRMax+(0.5*mm)); // Stop any chance of overlap with outer surface of the chamber;
        // Put in rotations
        // locModule.rotateZ(portRotations[0][0]);
        // locModule.rotateY(portRotations[0][1]);
        // locModule.rotateZ(portRotations[0][2]);

        // Rotate modules so they tesselate together
        if(i == 6 || i == 7){
            rotModule.setPhi((72.)*deg);
        }
        else{
            rotModule.setPhi((72.)*deg);
        }

        // Put module transform info into vector storage object
        transfModuleVec[i] = G4Transform3D(rotModule, locModule);
    }






    // --------------------
    // Volume construction
    // --------------------

    // ----------
    // World
    // ----------
    G4Box* solidWorld = new G4Box("World", worldXY, worldXY, worldZ); // Args: name, x 1/2 length, y 1/2 length, z 1/2 length.
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World"); // Args: solid volume (shape), material, name.
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, 0, 0, true); // New physical volume placement:
    // Args: rotation vector, translation vector, logical volume being placed, name, mother volume, boolean operations, copy number, overlap checking.
    
    // -------------------
    // Aluminium chamber
    // -------------------
    G4Sphere* solidChamberSph = new G4Sphere("ChamberSphere", chamberRMin, chamberRMax, 0., 360.*deg, 0., 180.*deg);
    G4Tubs* drillHole = new G4Tubs("Drill", 0., 1.1*BTRMax, 1.1*chamberRMax, 0., 360.*deg);
    G4SubtractionSolid* solidChamber = new G4SubtractionSolid("Chamber", solidChamberSph, drillHole);

    G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber, natAl, "Chamber");
    new G4PVPlacement(0, G4ThreeVector(), logicChamber, "Chamber", logicWorld, 0, 0, true);

    // --------------------
    // Aluminium beam tubes
    // --------------------
    G4Tubs* solidBT = new G4Tubs("BeamTube", BTRMin, BTRMax, BTZLen, 0., 360.*deg);
    // BT cones
    G4Cons* solidCons = new G4Cons("BTCones", BTConsRMin1, BTConsRMax1, BTConsRMin2, BTConsRMax2, BTConsZ, 0., 360.*deg);
    // Put cone on the cylindrical beam tube
    G4UnionSolid* solidBTCone = new G4UnionSolid("BeamTube", solidBT, solidCons, 0, BTConsLocVector);

    G4LogicalVolume* logicBT = new G4LogicalVolume(solidBTCone, natAl, "BeamTube");
    new G4PVPlacement(BTConeFlip, -BTLocVector, logicBT, "BeamTubeFront", logicWorld, 0, 0, true); // Upstream of target
    new G4PVPlacement(0, BTLocVector, logicBT, "BeamTubeBack", logicWorld, 0, 0, true); // Downstream of target


    // G4LogicalVolume* logicBTCons = new G4LogicalVolume(solidCons, natAl, "BTCones");
    // new G4PVPlacement(0, BTConsLocVector, logicBTCons, "BTConeBack", logicWorld, 0, 0, true);
    // new G4PVPlacement(BTConeFlip, -BTConsLocVector, logicBTCons, "BTConeFront", logicWorld, 0, 0, true);

    // -----------------
    // Aluminium shell
    // ----------------- 
    G4Sphere* solidShellSph = new G4Sphere("ShellSph", shellRMin, shellRMax, 270.*deg, 180.*deg, 0., 180.*deg); // Hemisphere. Angle args: start
    G4Tubs* beamPort = new G4Tubs("BeamPort", 0., smallPortR, 1.1*shellRMax, 0., 360.*deg);
    G4SubtractionSolid* solidShell = new G4SubtractionSolid("Shell", solidShellSph, beamPort, G4Transform3D::Identity); // Like coring an apple if it was hollow, aligned with Z

    // Module port (cylinder to cut into shell that's depth is larger than the shell thickness)
    G4Tubs* modulePort = new G4Tubs("ModulePort", 0., 1.05*modulePortR, (1.1*(shellRMax - shellRMin)), 0., 360.*deg); // Z length is a mean, but needed a x2 so no /2

    // Cut Module 0 hole into the shell
    G4SubtractionSolid* solidShellMod0 = new G4SubtractionSolid("Shell", solidShell, modulePort, transfPortVec[0]);
    // Cut Module 1 hole
    G4SubtractionSolid* solidShellMod1 = new G4SubtractionSolid("Shell", solidShellMod0, modulePort, transfPortVec[1]);
    // Cut Module 2 hole (slot 5)
    G4SubtractionSolid* solidShellMod2 = new G4SubtractionSolid("Shell", solidShellMod1, modulePort, transfPortVec[2]);
    // Cut Module 3 hole (slot 6)
    G4SubtractionSolid* solidShellMod3 = new G4SubtractionSolid("Shell", solidShellMod2, modulePort, transfPortVec[3]);
    // Cut Module 4 hole (slot 10)
    G4SubtractionSolid* solidShellMod4 = new G4SubtractionSolid("Shell", solidShellMod3, modulePort, transfPortVec[4]);
    // Cut Module 5 hole (slot 11)
    G4SubtractionSolid* solidShellMod5 = new G4SubtractionSolid("Shell", solidShellMod4, modulePort, transfPortVec[5]);
    // Cut Module 6 hole (slot 12)
    G4SubtractionSolid* solidShellMod6 = new G4SubtractionSolid("Shell", solidShellMod5, modulePort, transfPortVec[6]);
    // Cut Module 7 hole (slot 13)
    G4SubtractionSolid* solidShellMod7 = new G4SubtractionSolid("Shell", solidShellMod6, modulePort, transfPortVec[7]);
    // Cut Module 8 hole (slot 14)
    G4SubtractionSolid* solidShellMod8 = new G4SubtractionSolid("Shell", solidShellMod7, modulePort, transfPortVec[8]);
    // Cut Module 9 hole (slot 20)
    G4SubtractionSolid* solidShellMod9 = new G4SubtractionSolid("Shell", solidShellMod8, modulePort, transfPortVec[9]);
    // Cut Module 10 hole (slot 21)
    G4SubtractionSolid* solidShellMod10 = new G4SubtractionSolid("Shell", solidShellMod9, modulePort, transfPortVec[10]);
    // Cut Module 11 hole (slot 25)
    G4SubtractionSolid* solidShellMod11 = new G4SubtractionSolid("Shell", solidShellMod10, modulePort, transfPortVec[11]);
    // Cut Module 12 hole (slot 26)
    G4SubtractionSolid* solidShellMod12 = new G4SubtractionSolid("Shell", solidShellMod11, modulePort, transfPortVec[12]);

    G4LogicalVolume* logicShell = new G4LogicalVolume(solidShellMod1, natAl, "Shell");
    new G4PVPlacement(0, G4ThreeVector(), logicShell, "Shell", logicWorld, 0, 0, true);

    // -------------------
    // Germanium hex & cylinder
    // -------------------
    // Full phi coverage, 6 sided regular hex cross section, expanding outwards 10 deg
    G4Polyhedra* solidGeHex = new G4Polyhedra("GeHex", 30.*deg, phiMax, nSides, nZPlanes, locZPlanes, rInner, rOuter);

    // 80 mm diameter, 90 mm long cylinder
    G4Tubs* solidGeOuterCyl = new G4Tubs("GeOuterCyl", 0., GeCylROuter, GeCylZ, 0., phiMax);

    // 18cm radius sphere to take subtraction on the front face
    G4Sphere* subtractionSph = new G4Sphere("Subtraction", 0, chamberRMax, 0, 360.*deg, 0., 180.*deg);

    // Coaxial cutout. Take a sphere and a cylinder away from the larger cylinder
    G4Sphere* solidCoaxSph = new G4Sphere("GeCoaxSph", 0., rCoax, 0., 360.*deg, 0., 180.*deg);
    G4Tubs* solidCoaxCyl = new G4Tubs("GeCoaxCyl", 0., rCoax, coaxCylZ, 0., phiMax);

    // Cyl + sphere = coax
    G4UnionSolid* solidGeCoaxCutout = new G4UnionSolid(GeName, solidCoaxSph, solidCoaxCyl, 0, coaxCylCen);
    // Big cyl - coax
    G4SubtractionSolid* solidGeCoaxCyl2 = new G4SubtractionSolid(GeName, solidGeOuterCyl, solidGeCoaxCutout, 0, coaxSphCen);

    // Union of the cylinder (with cutout) and the hexagon
    G4IntersectionSolid* solidGeHexCyl = new G4IntersectionSolid(GeName, solidGeHex, solidGeCoaxCyl2);

    // First crystal
    G4MultiUnion* solidQuadGe = new G4MultiUnion(GeName);
    solidQuadGe->AddNode(solidGeHexCyl, transf0to1);
    // Add crystal 2
    solidQuadGe->AddNode(solidGeHexCyl, transf0to2);
    // Add crystal 3
    solidQuadGe->AddNode(solidGeHexCyl, transf0to3);
    // Add crystal 4
    solidQuadGe->AddNode(solidGeHexCyl, transf0to4);
    // Voxelise
    solidQuadGe->Voxelize();

    // Subtract 18cm sphere from front face
    G4SubtractionSolid* solidCloverGe = new G4SubtractionSolid("GeClover", solidQuadGe, subtractionSph, 0, subSphLoc);

    // G4LogicalVolume* logicGe = new G4LogicalVolume(solidQuadGe, natGe, "GeClover");
    G4LogicalVolume* logicGe = new G4LogicalVolume(solidCloverGe, natGe, "GeClover");


    // Place Germanium of module 0
    fGermaniumPV = new G4PVPlacement(transfModuleVec[0], logicGe, "GeClover", logicWorld, 0, 0, true);
    // Subsequent modules
    for(int i = 0; i < nModules; i++){
        new G4PVPlacement(transfModuleVec[i], logicGe, "GeClover", logicWorld, 0, 0, true);
    }

    // ---------------------
    // Copper contacts
    // ---------------------
    G4Tubs* solidCuContact = new G4Tubs("CuContact", 0., GeCylROuter, cuContactZ, 0., phiMax);
    G4MultiUnion* solidCuContacts = new G4MultiUnion();
    solidCuContacts->AddNode(solidCuContact, transfContact1);
    solidCuContacts->AddNode(solidCuContact, transfContact2);
    solidCuContacts->AddNode(solidCuContact, transfContact3);
    solidCuContacts->AddNode(solidCuContact, transfContact4);
    solidCuContacts->Voxelize();

    G4LogicalVolume* logicContacts = new G4LogicalVolume(solidCuContacts, natCu, "CuContacts");

    // Place Cu contacts of modules
    for(int i = 0; i < nModules; i++){
        new G4PVPlacement(transfModuleVec[i], logicContacts, "CuContacts", logicWorld, 0, 0, true);
    }

    // ----------------
    // Module backend
    // ----------------

    // FET compartment
    G4Box* fetBoxS = new G4Box("BoxSmall", fetSquLen1x, fetSquLen1y, fetZ);
    G4Box* fetBoxL = new G4Box("BoxLarge", fetSquLen2, fetSquLen2, fetZ);
    // Union. Rotate box L by 45 deg in Z.
    G4IntersectionSolid* solidFet = new G4IntersectionSolid("FETCompartment", fetBoxS, fetBoxL, z45Rot, G4ThreeVector());

    // Preamp compartment
    G4Tubs* solidPreamp = new G4Tubs("PreampCompartment", 0, preampR, preampZ, 0, 360.*deg);

    // Next section (4)
    G4Tubs* solidSec4 = new G4Tubs("Sec4", 0, sec4R, sec4Z, 0, 360.*deg);

    // Section 4 stopper ring
    G4Tubs* solidStopper = new G4Tubs("Stopper", sec4R, stopperR, stopperZ, 0, 360.*deg);

    // Next section (5)
    G4Tubs* solidSec5 = new G4Tubs("Sec5", 0, sec5R, sec5Z, 0, 360.*deg);

    // Dewar
    G4Tubs* solidDewar = new G4Tubs("Dewar", 0, dewarR, dewarZ, 0, 360.*deg);

    // Put all Al parts together. They are all placed along Z, and share the same origin as the crystals (none of these parts start at 0,0,0).
    G4MultiUnion* solidModule = new G4MultiUnion("ModuleAl");
    solidModule->AddNode(solidFet, G4Transform3D(G4RotationMatrix::IDENTITY,fetLoc));
    solidModule->AddNode(solidPreamp, G4Transform3D(G4RotationMatrix::IDENTITY,preampLoc));
    solidModule->AddNode(solidSec4, G4Transform3D(G4RotationMatrix::IDENTITY,sec4Loc));
    solidModule->AddNode(solidStopper, G4Transform3D(G4RotationMatrix::IDENTITY,stopperLoc));
    solidModule->AddNode(solidSec5, G4Transform3D(G4RotationMatrix::IDENTITY,sec5Loc));
    solidModule->AddNode(solidDewar, G4Transform3D(G4RotationMatrix::IDENTITY,dewarLoc));

    solidModule->Voxelize();

    G4LogicalVolume* logicModule = new G4LogicalVolume(solidModule, natAl, "ModuleAl");

    // Place backend of modules
    for(int i = 0; i < nModules; i++){
        new G4PVPlacement(transfModuleVec[i], logicModule, "ModuleAl", logicWorld, 0, 0, true);
    }

    return physWorld;
}