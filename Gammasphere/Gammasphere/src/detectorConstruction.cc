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
    G4Material* natAl = nist->FindOrBuildMaterial("G4_Al");

    // Crystal = natural Germanium
    G4Material* natGe = nist->FindOrBuildMaterial("G4_Ge");

    // Contacts = natural copper
    G4Material* natCu = nist->FindOrBuildMaterial("G4_Cu");

    // Collimator = Hevimet
    // Elements needed: W, Cu, Ni
    G4Element* el_W = new G4Element("Tungsten", "W", 74., 183.8*g/mole); // Args: label, symbol, Z (G4double), avg A (G4double)
    G4Element* el_Cu = new G4Element("Copper", "Cu", 29., 63.55*g/mole);
    G4Element* el_Ni = new G4Element("Nickel", "Ni", 28., 58.69*g/mole);
    // Put these together
    G4Material* Hevimet = new G4Material("Hevimet", 18.23*g/cm3, 3); // Args: label, density, nComponents
    Hevimet->AddElement(el_W, 0.9);
    Hevimet->AddElement(el_Cu, 0.04);
    Hevimet->AddElement(el_Ni, 0.06);

    // Compton suppression = BGO
    G4Element* el_Bi = new G4Element("Bismuth", "Bi", 83., 208.98*g/mole);
    G4Element* el_Ge = new G4Element("Germanium", "Ge", 32., 72.64*g/mole);
    G4Element* el_O = new G4Element("Oxygen", "O", 8., 15.99*g/mole);
    // BGO = Bi(4)Ge(3)O(12)
    G4Material* BGO = new G4Material("BGO", 7.13*g/cm3, 3);
    BGO->AddElement(el_Bi, 4);
    BGO->AddElement(el_Ge, 3);
    BGO->AddElement(el_O, 12);

    // --------------------
    // Parameters
    // --------------------
    // Change these variables to change key parameters of the geometry. Geant uses half-lengths most of the time.
    // Z axis aligns the source and detector.
    
    // #########################
    bool if_one_module = false;
    // #########################

    G4double worldXY = 150*cm;
    G4double worldZ = 250*cm;
    
    G4RotationMatrix* pRotIden = new G4RotationMatrix();
    G4RotationMatrix* pConeFlip = new G4RotationMatrix();
    pConeFlip->rotateY(180.*deg);

    // ----- HEVIMET Collimator -----
    G4double zFrontFace = 21.8*cm;
    G4double zColl = 2.5*cm;
    // Polyhedra (Hex)
    G4double locZPlanesColl[] = {zFrontFace-zColl, zFrontFace};
    G4double rZero[] = {0., 0.};
    G4double rOuterColl[] = {((3.673-0.2)/1.1547)*cm, ((4.-0.2)/1.1547)*cm};
    // Cone (inner surface)
    G4double rInnerColl[] = {2.445*cm, 2.772*cm};
    G4double rLarge[] = {7.*cm, 7.3*cm};

    // ----- Germanium -----
    G4double zGeFront = 25*cm; // 24.6
    G4double GeDepth1 = 2.*cm;
    G4double GeDepth2 = 6.4*cm; // 5.5
    G4double GeRadOuter1 = 3.238*cm;
    G4double GeRadOuter2 = 3.6*cm; // 3.5
    G4double GeRadInner = 0.6*cm;

    // ----- Copper -----
    G4double plateDepth = 1.*cm;
    G4double rElec1 = 4.*cm;
    G4double rElec2 = 5.*cm;
    G4double elecDepth1 = 14.5*cm;
    G4double elecDepth2 = 6.*cm;
    G4double zBGOtoElec = 9.*cm;

    // ----- BGO -----
    // Cyl
    G4double rBGOCyl = 2.5*cm;
    G4double BGOCylDepth = 4.*cm;
    G4double zPlatetoBGO = 1.5*cm;
    G4double xOffsetBGO = 1.*cm;
    // Wedge: front
    G4double BGOFrontDepth = 2.3*cm;
    G4double locZPlanesBGOFront[] = {zFrontFace, zFrontFace+BGOFrontDepth};
    G4double rOuterBGOFront[] = {rOuterColl[1], ((4.301-0.2)/1.1547)*cm};
    G4double rInnerBGOFront[] = {rInnerColl[1], (3.171-0.2)*cm};
    // Wedge: mid
    G4double BGOMidConDepth = 2.5*cm;
    G4double BGOMidCylDepth = 13.*cm;
    G4double rOuterBGOMid[] = {rOuterBGOFront[1], (6.228/1.1547)*cm};
    G4double rInnerBGOMid[] = {(3.988-0.35-0.3)*cm, (4.25-0.35-0.2)*cm};
    G4double locZPlanesBGOMid[] = {zFrontFace+BGOFrontDepth, zFrontFace+BGOFrontDepth+BGOMidConDepth+BGOMidCylDepth}; // Doesn't include middle plane of the can

    G4double zBGOCylFront = zGeFront+GeDepth1+GeDepth2+plateDepth+zPlatetoBGO;

    // ----- Al -----
    // Front hex
    G4double tolerance = 0.01*mm;
    G4double rOuterAlFront[] = {rOuterColl[0]+0.5*cm+tolerance-0.3*cm, rOuterBGOMid[1]+0.5*cm+tolerance-0.3*cm};
    G4double rInnerAlFront[] = {rOuterColl[0]+tolerance, rOuterBGOMid[1]+tolerance};
    G4double locZPlanesAlFront[] = {zFrontFace-zColl, zFrontFace+BGOFrontDepth+BGOMidConDepth+BGOMidCylDepth};
    // Mid hex
    G4double zAlMid = zFrontFace+BGOFrontDepth+BGOMidConDepth+BGOMidCylDepth;
    G4double locZPlanesAlMid[] = {zAlMid, zAlMid+0.5*cm, zAlMid+20.*cm};
    G4double rOuterAlMid[] = {rOuterAlFront[1], rOuterAlFront[1], rOuterAlFront[1]};
    G4double rInnerAlMid[] = {rOuterAlFront[1], rOuterAlFront[1]-1.*cm, rOuterAlFront[1]-1.*cm};
    // Rear cylinders
    G4double zAlCyl = locZPlanesAlMid[2];
    G4double AlCyl1Depth = 2.54*cm;
    G4double AlCyl2Depth = 12.54*cm;
    G4double AlCyl3Depth = 2.54*cm;
    G4double rAlCyl1[] = {((6.328/1.1547)*cm+tolerance), ((6.328/1.1547)*cm+tolerance+(1*2.54)*cm)};
    G4double rAlCyl2[] = {((6.328/1.1547)*cm+tolerance+(0.5*2.54)*cm), ((6.328/1.1547)*cm+tolerance+(1*2.54)*cm)};
    G4double rAlCyl3[] = {((6.328/1.1547)*cm+tolerance+(1*2.54)*cm), ((6.328/1.1547)*cm+tolerance+(3*2.54)*cm-(3.*mm))};
    // Tube
    G4double rAlTube[] = {0.9*cm, 1.*cm};
    G4double AlTubeDepth = 13.5*cm;
    G4double AlCapDepth = 0.1*cm;
    // Front cap (end cap so Ge isn't exposed)
    // Reuse other G4doubles

    // ----- Shell -----
    G4double shellRMin = zAlCyl + AlCyl1Depth;
    G4double shellRMax = zAlCyl + AlCyl1Depth + AlCyl2Depth - AlCyl3Depth - tolerance;
    G4double rModule = rAlCyl2[1] + tolerance;

    // ----- Transformations -----
    // Rotations. Beamline on Z axis is the reference point for theta. Phi = 0 is on the x-axis and subtends around towards y.
    // Rotation API: https://apc.u-paris.fr/~franco/g4doxy4.10/html/class_c_l_h_e_p_1_1_hep_rotation.html
    // 3-Vec API: https://apc.u-paris.fr/~franco/g4doxy4.10/html/class_c_l_h_e_p_1_1_hep3_vector.html
    int nModules = 110;
    int nModules_used = 100;
    // G4double rotationVec[] = {17.27465*deg, 72.*deg}; // Theta(GEB), Phi(GEB)
    // 2D C++ vector to store all the theta(GEB)s, phi(GEB)s
    // Idx 1 = DetID - 1, Idx 2 = theta (0) or phi (1)
    G4double rotations [nModules][2];
    // ----- Downstream -----
    // Angid = 1
    rotations[0][0] = 17.27465*deg;
    rotations[1][0] = 17.27465*deg;
    rotations[2][0] = 17.27465*deg;
    rotations[3][0] = 17.27465*deg;
    rotations[5][0] = 17.27465*deg;
    rotations[0][1] = 72.*deg;
    rotations[1][1] = 216.*deg;
    rotations[2][1] = 144.*deg;
    rotations[3][1] = 288.*deg;
    rotations[5][1] = 0.;
    // Angid = 2
    rotations[4][0] = 31.71747*deg;
    rotations[6][0] = 31.71747*deg;
    rotations[7][0] = 31.71747*deg;
    rotations[8][0] = 31.71747*deg;
    rotations[9][0] = 31.71747*deg;
    rotations[4][1] = 36.*deg;
    rotations[6][1] = 108.*deg;
    rotations[7][1] = 252.*deg;
    rotations[8][1] = 180.*deg;
    rotations[9][1] = 324.*deg;
    // Angid = 3
    rotations[10][0] = 37.37737*deg;
    rotations[11][0] = 37.37737*deg;
    rotations[12][0] = 37.37737*deg;
    rotations[13][0] = 37.37737*deg;
    rotations[15][0] = 37.37737*deg;
    rotations[10][1] = 72.*deg;
    rotations[11][1] = 216.*deg;
    rotations[12][1] = 144.*deg;
    rotations[13][1] = 288.*deg;
    rotations[15][1] = 0;
    // Angid = 4
    rotations[14][0] = 50.06504*deg;
    rotations[16][0] = 50.06504*deg;
    rotations[17][0] = 50.06504*deg;
    rotations[18][0] = 50.06504*deg;
    rotations[19][0] = 50.06504*deg;
    rotations[20][0] = 50.06504*deg;
    rotations[21][0] = 50.06504*deg;
    rotations[22][0] = 50.06504*deg;
    rotations[23][0] = 50.06504*deg;
    rotations[25][0] = 50.06504*deg;
    rotations[14][1] = 22.84*deg;
    rotations[16][1] = 49.16*deg;
    rotations[17][1] = 193.16*deg;
    rotations[18][1] = 94.84*deg;
    rotations[19][1] = 238.84*deg;
    rotations[20][1] = 121.16*deg;
    rotations[21][1] = 265.16*deg;
    rotations[22][1] = 166.84*deg;
    rotations[23][1] = 310.84*deg;
    rotations[25][1] = 337.16*deg;
    // Angid = 5
    rotations[24][0] = 58.28253*deg;
    rotations[26][0] = 58.28253*deg;
    rotations[27][0] = 58.28253*deg;
    rotations[29][0] = 58.28253*deg;
    rotations[31][0] = 58.28253*deg;
    rotations[24][1] = 72.*deg;
    rotations[26][1] = 144.*deg;
    rotations[27][1] = 216.*deg;
    rotations[29][1] = 288.*deg;
    rotations[31][1] = 0;
    // Angid = 6
    rotations[28][0] = 69.82033*deg;
    rotations[30][0] = 69.82033*deg;
    rotations[32][0] = 69.82033*deg;
    rotations[33][0] = 69.82033*deg;
    rotations[34][0] = 69.82033*deg;
    rotations[35][0] = 69.82033*deg;
    rotations[36][0] = 69.82033*deg;
    rotations[37][0] = 69.82033*deg;
    rotations[39][0] = 69.82033*deg; // change to 39
    rotations[41][0] = 69.82033*deg; // 41
    rotations[28][1] = 18.49*deg;
    rotations[30][1] = 53.51*deg;
    rotations[32][1] = 90.49*deg;
    rotations[33][1] = 197.51*deg;
    rotations[34][1] = 125.51*deg;
    rotations[35][1] = 234.49*deg;
    rotations[36][1] = 162.49*deg;
    rotations[37][1] = 269.51*deg;
    rotations[39][1] = 306.49*deg; // change to 39
    rotations[41][1] = 341.51*deg; // 41
    // Angid = 7
    rotations[38][0] = 79.18768*deg;
    rotations[40][0] = 79.18768*deg;
    rotations[43][0] = 79.18768*deg; // change to 43
    rotations[45][0] = 79.18768*deg; // change to 45
    rotations[47][0] = 79.18768*deg; // change to 47
    rotations[38][1] = 72.*deg;
    rotations[40][1] = 144.*deg;
    rotations[43][1] = 216.*deg; // change to 43
    rotations[45][1] = 288.*deg; // change to 45
    rotations[47][1] = 0.*deg; // change to 47
    // Angid = 8
    rotations[42][0] = 80.7096*deg;
    rotations[44][0] = 80.7096*deg;
    rotations[46][0] = 80.7096*deg;
    rotations[49][0] = 80.7096*deg; // change to 49
    rotations[51][0] = 80.7096*deg; // change to 51
    rotations[42][1] = 36.*deg;
    rotations[44][1] = 108.*deg;
    rotations[46][1] = 180.*deg;
    rotations[49][1] = 252.*deg; // change to 49
    rotations[51][1] = 324.*deg; // change to 51
    // ----- Boundary -----
    // Angid = 9
    rotations[48][0] = 90.*deg;
    rotations[50][0] = 90.*deg;
    rotations[52][0] = 90.*deg;
    rotations[53][0] = 90.*deg;
    rotations[54][0] = 90.*deg;
    rotations[55][0] = 90.*deg;
    rotations[56][0] = 90.*deg;
    rotations[57][0] = 90.*deg;
    rotations[59][0] = 90.*deg; // change to 59
    rotations[61][0] = 90.*deg; // change to 61
    rotations[48][1] = 18.*deg;
    rotations[50][1] = 54.*deg;
    rotations[52][1] = 90.*deg;
    rotations[53][1] = 198.*deg;
    rotations[54][1] = 126.*deg;
    rotations[55][1] = 234.*deg;
    rotations[56][1] = 162.*deg;
    rotations[57][1] = 270.*deg;
    rotations[59][1] = 306.*deg; // change to 59
    rotations[61][1] = 342.*deg; // change to 61
    // Angid = 10
    rotations[58][0] = 99.2904*deg;
    rotations[60][0] = 99.2904*deg;
    rotations[63][0] = 99.2904*deg; // change to 63
    rotations[65][0] = 99.2904*deg; // change to 65
    rotations[67][0] = 99.2904*deg; // change to 67
    rotations[58][1] = 72.*deg;
    rotations[60][1] = 144.*deg;
    rotations[63][1] = 216.*deg; // change to 63
    rotations[65][1] = 288.*deg; // change to 65
    rotations[67][1] = 0.*deg; // change to 67
    // ----- Upstream -----
    // Angid = 11
    rotations[62][0] = 100.81232*deg;
    rotations[64][0] = 100.81232*deg;
    rotations[66][0] = 100.81232*deg;
    rotations[69][0] = 100.81232*deg; // change to 69
    rotations[71][0] = 100.81232*deg; // change to 71
    rotations[62][1] = 36.*deg;
    rotations[64][1] = 108.*deg;
    rotations[66][1] = 180.*deg;
    rotations[69][1] = 252.*deg; // change to 69
    rotations[71][1] = 324.*deg; // change to 71
    // Angid = 12
    rotations[68][0] = 110.17967*deg;
    rotations[70][0] = 110.17967*deg;
    rotations[72][0] = 110.17967*deg;
    rotations[73][0] = 110.17967*deg;
    rotations[74][0] = 110.17967*deg;
    rotations[75][0] = 110.17967*deg;
    rotations[76][0] = 110.17967*deg;
    rotations[77][0] = 110.17967*deg;
    rotations[79][0] = 110.17967*deg; // change to 79
    rotations[81][0] = 110.17967*deg; // change to 81
    rotations[68][1] = 17.51*deg;
    rotations[70][1] = 54.59*deg;
    rotations[72][1] = 89.51*deg;
    rotations[73][1] = 198.49*deg;
    rotations[74][1] = 126.49*deg;
    rotations[75][1] = 233.51*deg;
    rotations[76][1] = 161.51*deg;
    rotations[77][1] = 270.49*deg;
    rotations[79][1] = 305.51*deg; // change to 79
    rotations[81][1] = 342.49*deg; // change to 81
    // Angid = 13
    rotations[78][0] = 121.71747*deg;
    rotations[80][0] = 121.71747*deg;
    rotations[82][0] = 121.71747*deg;
    rotations[83][0] = 121.71747*deg;
    rotations[85][0] = 121.71747*deg; // change to 85
    rotations[78][1] = 36.*deg;
    rotations[80][1] = 108.*deg;
    rotations[82][1] = 180.*deg;
    rotations[83][1] = 252.*deg;
    rotations[85][1] = 324.*deg; // change to 85
    // Angid = 14
    rotations[84][0] = 129.93496*deg;
    rotations[86][0] = 129.93496*deg;
    rotations[87][0] = 129.93496*deg;
    rotations[88][0] = 129.93496*deg;
    rotations[89][0] = 129.93496*deg;
    rotations[90][0] = 129.93496*deg;
    rotations[91][0] = 129.93496*deg;
    rotations[92][0] = 129.93496*deg;
    rotations[93][0] = 129.93496*deg;
    rotations[95][0] = 129.93496*deg; // change to 95
    rotations[84][1] = 13.16*deg;
    rotations[86][1] = 58.84*deg;
    rotations[87][1] = 202.84*deg;
    rotations[88][1] = 85.16*deg;
    rotations[89][1] = 229.16*deg;
    rotations[90][1] = 130.84*deg;
    rotations[91][1] = 274.84*deg;
    rotations[92][1] = 157.16*deg;
    rotations[93][1] = 301.16*deg;
    rotations[95][1] = 346.84*deg; // change to 95
    // Angid = 15
    rotations[94][0] = 142.62263*deg;
    rotations[96][0] = 142.62263*deg;
    rotations[97][0] = 142.62263*deg;
    rotations[98][0] = 142.62263*deg;
    rotations[99][0] = 142.62263*deg;
    rotations[94][1] = 36.*deg;
    rotations[96][1] = 108.*deg;
    rotations[97][1] = 252.*deg;
    rotations[98][1] = 180.*deg;
    rotations[99][1] = 324.*deg;
    // Angid = 16
    rotations[100][0] = 148.28253*deg;
    rotations[101][0] = 148.28253*deg;
    rotations[102][0] = 148.28253*deg;
    rotations[103][0] = 148.28253*deg;
    rotations[105][0] = 148.28253*deg; // change to 105
    rotations[100][1] = 72.*deg;
    rotations[101][1] = 216.*deg;
    rotations[102][1] = 144.*deg;
    rotations[103][1] = 288.*deg;
    rotations[105][1] = 0.*deg; // change to 105
    // Angid = 17
    rotations[104][0] = 162.72535*deg;
    rotations[106][0] = 162.72535*deg;
    rotations[107][0] = 162.72535*deg;
    rotations[108][0] = 162.72535*deg;
    rotations[109][0] = 162.72535*deg;
    rotations[104][1] = 36.*deg;
    rotations[106][1] = 108.*deg;
    rotations[107][1] = 252.*deg;
    rotations[108][1] = 180.*deg;
    rotations[109][1] = 324.*deg;

    G4ThreeVector posMod;
    G4RotationMatrix* pRotMod; 

    // ###############################################################################################################################################

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
    
    // ----------
    // Collimator
    // ----------
    // Intersection of a hex prism (contains desired outer surface) and a cylinder (contains inner surface)
    G4Polyhedra* solidCollHex = new G4Polyhedra("CollHex", 0, 360.*deg, 6, 2, locZPlanesColl, rZero, rOuterColl);
    // Args: label, start ang, ang coverage, nSides, nPlanes, Z of planes, vec of inner rads, vec of outer rads
    G4Cons* solidCollCone = new G4Cons("CollCone", rInnerColl[0], rLarge[0], rInnerColl[1], rLarge[1], zColl/2., 0, 360.*deg);
    // Args: label, RMin1, RMax1, RMin2, RMax2, length, start angle, ang coverage
    // Put together
    // Intersection Solid is not very overloaded so it expects very particular types i.e. rotation must be a pointer
    G4IntersectionSolid* solidColl = new G4IntersectionSolid(G4String("Collimator"), solidCollHex, solidCollCone, pRotIden, G4ThreeVector(0, 0, zFrontFace - (zColl/2)));
    G4LogicalVolume* logicColl = new G4LogicalVolume(solidColl, Hevimet, "Collimator");
    if(if_one_module){
        new G4PVPlacement(0, G4ThreeVector(), logicColl, "Collimator", logicWorld, 0, 0, true);
    }
    // ----------
    // Germanium
    // ----------
    // Union of a cone and a hollow cylinder. MultiUnion instead of Union so the origin of the shape can be controlled easily and made to match GAMMASPHERE origin.
    G4Cons* GeFront = new G4Cons("GeFront", 0, GeRadOuter1, 0, GeRadOuter2, GeDepth1/2., 0, 360.*deg);
    G4Tubs* GeBack = new G4Tubs("GeBack", GeRadInner, GeRadOuter2, GeDepth2/2., 0, 360.*deg);
    // Put together
    G4MultiUnion* solidGe = new G4MultiUnion("solidGe");
    solidGe->AddNode(GeFront, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zGeFront + (GeDepth1 /2.)) ));
    solidGe->AddNode(GeBack, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zGeFront + GeDepth1 + (GeDepth2 /2.)) ));
    solidGe->Voxelize();
    G4LogicalVolume* logicGe = new G4LogicalVolume(solidGe, natGe, "GeCrystal");
    // Pointer needed for the EDep tally
    if(if_one_module){
        fGermaniumPV = new G4PVPlacement(0, G4ThreeVector(), logicGe, "GeCrystal", logicWorld, 0, 0, true);
    }
    // -------
    // Copper
    // -------
    // MultiUnion of plate behind Ge and 'electronics' on technical drawings.
    // Plate
    G4Tubs* CuPlate = new G4Tubs("CuPlate", 0, GeRadOuter2, plateDepth/2., 0, 360.*deg);
    // Electronics
    G4Tubs* CuElec1 = new G4Tubs("CuElec1", 0, rElec1, elecDepth1/2., 0, 360.*deg);
    G4Tubs* CuElec2 = new G4Tubs("CuElec2", 0, rElec2, elecDepth2/2., 0, 360.*deg);
    // Put together
    G4MultiUnion* solidCu = new G4MultiUnion("solidCu");
    solidCu->AddNode(CuPlate, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, (zGeFront+GeDepth1+GeDepth2+(plateDepth/2.)) ) ));
    solidCu->AddNode(CuElec1, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, (zBGOCylFront+BGOCylDepth+zBGOtoElec+(elecDepth1/2.)) ) ));
    solidCu->AddNode(CuElec2, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, (zBGOCylFront+BGOCylDepth+zBGOtoElec+elecDepth1+(elecDepth2/2.)) ) ));
    solidCu->Voxelize();
    G4LogicalVolume* logicCu = new G4LogicalVolume(solidCu, natCu, "Cu");
    if(if_one_module){
        new G4PVPlacement(0, G4ThreeVector(), logicCu, "Cu", logicWorld, 0, 0, true);
    }
    // -----
    // BGO
    // -----
    // MultiUnion of offset cylinder behind Ge and side wedges.
    G4Tubs* solidBGOCyl = new G4Tubs("BGOCyl", 0, rBGOCyl, BGOCylDepth/2., 0, 360.*deg);
    // Wedges
    // Front (Hex outer, cyl inner, like the collimator)
    G4Polyhedra* solidBGOFrontHex = new G4Polyhedra("BGOFrontHex", 0, 360.*deg, 6, 2, locZPlanesBGOFront, rZero, rOuterBGOFront);
    G4Cons* solidBGOFrontCon = new G4Cons("BGOFrontCon", rInnerBGOFront[0], rLarge[0], rInnerBGOFront[1], rLarge[1], BGOFrontDepth/2., 0, 360.*deg);
    G4IntersectionSolid* solidBGOFront = new G4IntersectionSolid(G4String("BGOFront"), solidBGOFrontHex, solidBGOFrontCon, pRotIden, G4ThreeVector(0, 0, (zFrontFace + (BGOFrontDepth/2.))) );
    
    // Mid (Hex outer, can shaped inner). Can = hollow cone + tube.
    G4Polyhedra* solidBGOMidHex = new G4Polyhedra("BGOMidHex", 0, 360.*deg, 6, 2, locZPlanesBGOMid, rZero, rOuterBGOMid);
    G4Cons* solidBGOMidCon = new G4Cons("BGOMidCon", rInnerBGOMid[0], rLarge[0], rInnerBGOMid[1], rLarge[1], BGOMidConDepth/2., 0, 360.*deg);
    G4Tubs* solidBGOMidCyl = new G4Tubs("BGOMidCyl", rInnerBGOMid[1], rLarge[1], BGOMidCylDepth/2., 0, 360.*deg);
    // Con + Cyl = Can
    G4UnionSolid* solidBGOMidCan = new G4UnionSolid("BGOMidCan", solidBGOMidCon, solidBGOMidCyl, 0, G4ThreeVector(0, 0, (BGOMidConDepth/2. + BGOMidCylDepth/2.)) );
    // Intersection of Can and Hex
    G4IntersectionSolid* solidBGOMid = new G4IntersectionSolid("BGOMid", solidBGOMidHex, solidBGOMidCan, 0, G4ThreeVector(0, 0, zFrontFace+BGOFrontDepth+(BGOMidConDepth/2.)));
    // Put together
    G4MultiUnion* solidBGO = new G4MultiUnion("BGO");
    solidBGO->AddNode(solidBGOCyl, G4Transform3D(G4RotationMatrix(), G4ThreeVector(xOffsetBGO, 0, (zBGOCylFront+(BGOCylDepth)/2.) ) ));
    solidBGO->AddNode(solidBGOFront, G4Transform3D());
    solidBGO->AddNode(solidBGOMid, G4Transform3D(G4RotationMatrix(), G4ThreeVector()));
    solidBGO->Voxelize();
    G4LogicalVolume* logicBGO = new G4LogicalVolume(solidBGO, BGO, "BGO");
    if(if_one_module){
        new G4PVPlacement(0, G4ThreeVector(), logicBGO, "BGO", logicWorld, 0, 0, true);
    }
    // ----------
    // Aluminium
    // ----------
    // MultiUnion of front hex polyhedras which covers the BGOs, the front of the electronics, cylinders which interface with the ports in the shell,
    // and the tube next to the BGO cylinder that's behind the Ge.
    G4Polyhedra* solidAlFrontHex = new G4Polyhedra("AlFrontHex", 0, 360.*deg, 6, 2, locZPlanesAlFront, rInnerAlFront, rOuterAlFront);
    G4Polyhedra* solidAlMidHex = new G4Polyhedra("AlMidHex", 0, 360.*deg, 6, 3, locZPlanesAlMid, rInnerAlMid, rOuterAlMid);
    // Rear cylinders
    G4Tubs* solidAlRearCyl1 = new G4Tubs("AlCyl1", rAlCyl1[0], rAlCyl1[1], AlCyl1Depth/2., 0, 360.*deg);
    G4Tubs* solidAlRearCyl2 = new G4Tubs("AlCyl2", rAlCyl2[0], rAlCyl2[1], AlCyl2Depth/2., 0, 360.*deg);
    G4Tubs* solidAlRearCyl3 = new G4Tubs("AlCyl3", rAlCyl3[0], rAlCyl3[1], AlCyl3Depth/2., 0, 360.*deg);
    // Tube
    G4Tubs* solidAlTube = new G4Tubs("AlTube", rAlTube[0], rAlTube[1], AlTubeDepth/2., 0, 360.*deg);
    G4Tubs* solidAlCap = new G4Tubs("AlCap", 0, rInnerBGOMid[1], AlCapDepth/2., 0, 360.*deg);
    // End cap for Ge
    G4Tubs* solidAlFrontCap = new G4Tubs("AlFrontCap", 0, rInnerBGOMid[0], AlCapDepth/2., 0, 360.*deg);
    // Put together
    G4MultiUnion* solidAl = new G4MultiUnion("Al");
    solidAl->AddNode(solidAlFrontHex, G4Transform3D());
    solidAl->AddNode(solidAlMidHex, G4Transform3D());
    solidAl->AddNode(solidAlRearCyl1, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zAlCyl+(AlCyl1Depth/2.)) ));
    solidAl->AddNode(solidAlRearCyl2, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zAlCyl+AlCyl1Depth+(AlCyl2Depth/2.)) ));
    solidAl->AddNode(solidAlRearCyl3, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zAlCyl+AlCyl1Depth+AlCyl2Depth-(AlCyl3Depth/2.)) ));
    solidAl->AddNode(solidAlTube, G4Transform3D(G4RotationMatrix(), G4ThreeVector(-3.5*cm+rAlTube[1], 0, zGeFront+GeDepth1+GeDepth2+2.0*cm+(AlTubeDepth/2.)) ));
    solidAl->AddNode(solidAlCap, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zGeFront+GeDepth1+GeDepth2+2.0*cm+(AlCapDepth/2.)) ));
    solidAl->AddNode(solidAlFrontCap, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, zFrontFace+BGOFrontDepth+(AlCapDepth/2.)) ));
    solidAl->Voxelize();
    G4LogicalVolume* logicAl = new G4LogicalVolume(solidAl, natAl, "Al");
    if(if_one_module){
        new G4PVPlacement(0, G4ThreeVector(), logicAl, "Al", logicWorld, 0, 0, true);
    }
    // else{
    //     new G4PVPlacement(pRotMod1, G4ThreeVector(), logicAl, "Al", logicWorld, 0, 0, true);
    // }

    // ---------------------------------------------------------------------------------------------------
    // Construct spherical shell and place modules at corresponding port angles if if_one_module is false
    // ---------------------------------------------------------------------------------------------------

    if(if_one_module == false){
        // Gammasphere shell
        G4Sphere* solidSph = new G4Sphere("solidSphere", shellRMin, shellRMax, 0.*deg, 360.*deg, 0., 180.*deg); 
        // Label, rMin, rMax, start theta, theta range, start phi, phi range
        // Module hole
        G4Tubs* solidModHole = new G4Tubs("moduleHole", 0, rModule, shellRMax-shellRMin, 0, 360.*deg);        
        G4SubtractionSolid* solidShell;

        // Initialisation
        posMod = G4ThreeVector(0, 0, 1.*cm);
        pRotMod = new G4RotationMatrix();
        // GEB -> G4 angles
        posMod.setRThetaPhi((shellRMax+shellRMin)/2., rotations[0][0], rotations[0][1]);
        pRotMod->rotateZ(90.*deg - rotations[0][1]);
        pRotMod->rotateX(rotations[0][0]);

        // First cutout of shell and module placement
        solidShell = new G4SubtractionSolid("solidShell", solidSph, solidModHole, pRotMod, posMod);
        // Ge
        fGermaniumPV = new G4PVPlacement(pRotMod, G4ThreeVector(), logicGe, "GeCrystal", logicWorld, 0, 0, true);
        // BGO
        new G4PVPlacement(pRotMod, G4ThreeVector(), logicBGO, "BGO", logicWorld, 0, 0, true);
        // Al
        new G4PVPlacement(pRotMod, G4ThreeVector(), logicAl, "Al", logicWorld, 0, 0, true);
        // Cu
        new G4PVPlacement(pRotMod, G4ThreeVector(), logicCu, "Cu", logicWorld, 0, 0, true);
        // Collimator
        new G4PVPlacement(pRotMod, G4ThreeVector(), logicColl, "Collimator", logicWorld, 0, 0, true);

        // Rest of cutouts in shell
        for(int i = 1; i < nModules; i++){
            // Initialisation for module position and module rotation (pointer) vectors
            posMod = G4ThreeVector(0, 0, 1.*cm);
            pRotMod = new G4RotationMatrix();
            // GEB -> G4 angles
            posMod.setRThetaPhi((shellRMax+shellRMin)/2., rotations[i][0], rotations[i][1]);
            pRotMod->rotateZ(90.*deg - rotations[i][1]);
            pRotMod->rotateX(rotations[i][0]);
            // ----- Downstream -----
            // Angid = 1 : No rotation
            // Angid = 2 : rotate about own axis 30 deg
            if((i == 4) || ((i >= 6) && (i < 10))){
                pRotMod->setPsi(30.*deg);
            }
            // Angid = 3 : No rotation
            // Angid = 4-set1 : rotate
            if((i == 14) || (i == 18) || (i == 22) || (i == 23) || (i == 19)){
                pRotMod->setPsi(-20*deg);
            }
            // Angid = 4-set2 : rotate
            if((i == 16) || (i == 20) || (i == 17) || (i == 21) || (i == 25)){
                pRotMod->setPsi(20.*deg);
            }
            // Angid = 5 : No rotation
            // Angid = 6-set1 : rotate
            if((i == 30) || (i == 34) || (i == 33) || (i == 37) || (i == 41)){
                pRotMod->setPsi(5.*deg);
            }
            // Angid = 6-set2 : rotate
            if((i == 32) || (i == 36) || (i == 35) || (i == 39) || (i == 28)){
                pRotMod->setPsi(-5.*deg);
            }
            // Angid = 7 : No rotation
            // Angid = 8 : No rotation
            // Angid = 9 : No rotation
            // ----- Upstream -----
            // Angid = 10 : No rotation
            // Angid = 11 : No rotation
            // Angid = 12 : No rotation
            // Angid = 13 : No rotation
            // Angid = 14-set1 : rotate
            if((i == 84) || (i == 88) || (i == 92) || (i == 89) || (i == 93)){
                pRotMod->setPsi(-18.*deg);
            }
            // Angid = 14-set2 : rotate
            if((i == 95) || (i == 86) || (i == 90) || (i == 87) || (i == 91)){
                pRotMod->setPsi(18.*deg);
            }
            // Angid = 15 : No rotation
            // Angid = 16 : rotate
            if((i == 100) || (i == 101) || (i == 102) || (i == 103) || (i == 105)){
                pRotMod->setPsi(30.*deg);
            }
            // Angid = 17 : No rotation

            // Shell = Shell - hole
            solidShell = new G4SubtractionSolid("solidShell", solidShell, solidModHole, pRotMod, posMod);
        }


        // Iterate over modules
        for(int i = 1; i < nModules_used; i++){
            // Initialisation for module position and module rotation (pointer) vectors
            posMod = G4ThreeVector(0, 0, 1.*cm);
            pRotMod = new G4RotationMatrix();
            // GEB -> G4 angles
            posMod.setRThetaPhi((shellRMax+shellRMin)/2., rotations[i][0], rotations[i][1]);
            pRotMod->rotateZ(90.*deg - rotations[i][1]);
            pRotMod->rotateX(rotations[i][0]);
            // ----- Downstream -----
            // Angid = 1 : No rotation
            // Angid = 2 : rotate about own axis 30 deg
            if((i == 4) || ((i >= 6) && (i < 10))){
                pRotMod->setPsi(30.*deg);
            }
            // Angid = 3 : No rotation
            // Angid = 4-set1 : rotate
            if((i == 14) || (i == 18) || (i == 22) || (i == 23) || (i == 19)){
                pRotMod->setPsi(-20*deg);
            }
            // Angid = 4-set2 : rotate
            if((i == 16) || (i == 20) || (i == 17) || (i == 21) || (i == 25)){
                pRotMod->setPsi(20.*deg);
            }
            // Angid = 5 : No rotation
            // Angid = 6-set1 : rotate
            if((i == 30) || (i == 34) || (i == 33) || (i == 37) || (i == 41)){
                pRotMod->setPsi(5.*deg);
            }
            // Angid = 6-set2 : rotate
            if((i == 32) || (i == 36) || (i == 35) || (i == 39) || (i == 28)){
                pRotMod->setPsi(-5.*deg);
            }
            // Angid = 7 : No rotation
            // Angid = 8 : No rotation
            // Angid = 9 : No rotation
            // ----- Upstream -----
            // Angid = 10 : No rotation
            // Angid = 11 : No rotation
            // Angid = 12 : No rotation
            // Angid = 13 : No rotation
            // Angid = 14-set1 : rotate
            if((i == 84) || (i == 88) || (i == 92) || (i == 89) || (i == 93)){
                pRotMod->setPsi(-18.*deg);
            }
            // Angid = 14-set2 : rotate
            if((i == 95) || (i == 86) || (i == 90) || (i == 87) || (i == 91)){
                pRotMod->setPsi(18.*deg);
            }
            // Angid = 15 : No rotation
            // Angid = 16 : rotate
            if((i == 100) || (i == 101) || (i == 102) || (i == 103) || (i == 105)){
                pRotMod->setPsi(30.*deg);
            }
            // Angid = 17 : No rotation

            // Shell = Shell - hole
            // solidShell = new G4SubtractionSolid("solidShell", solidShell, solidModHole, pRotMod, posMod);
            // Place remaining volumes
            // Ge
            new G4PVPlacement(pRotMod, G4ThreeVector(), logicGe, "GeCrystal", logicWorld, 0, 0, true);
            // BGO
            new G4PVPlacement(pRotMod, G4ThreeVector(), logicBGO, "BGO", logicWorld, 0, 0, true);
            // Al
            new G4PVPlacement(pRotMod, G4ThreeVector(), logicAl, "Al", logicWorld, 0, 0, true);
            // Cu
            new G4PVPlacement(pRotMod, G4ThreeVector(), logicCu, "Cu", logicWorld, 0, 0, true);
            // Collimator
            new G4PVPlacement(pRotMod, G4ThreeVector(), logicColl, "Collimator", logicWorld, 0, 0, true);
        }

        // Shell placement
        G4LogicalVolume* logicShell = new G4LogicalVolume(solidShell, natAl, "Shell");
        new G4PVPlacement(0, G4ThreeVector(), logicShell, "Shell", logicWorld, 0, 0, true);
    }
    
    return physWorld;
}