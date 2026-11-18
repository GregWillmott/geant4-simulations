// Set up our geometry class for the .cc and main.cc to pull from
#ifndef detectorConstruction_h
#define detectorConstruction_h 1

// G4 libraries needed
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

// Classes from Geant
class G4VPhysicalVolume;
class G4LogicalVolume;

// Our detector construction class inherits from the Geant class, at public level
class OurDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        OurDetectorConstruction() = default; // Con
        ~OurDetectorConstruction() override = default; // Des
        G4VPhysicalVolume* Construct() override; // Function from Geant

        // Extra member, declaration here, but definition at bottom of file
        const G4VPhysicalVolume* GetGermaniumPV() const;
    private:
        G4VPhysicalVolume* fGermaniumPV = nullptr; // Pointer which will point to the germanium Phys Volume.
        // Purpose is to be sent to be available in the stepping action when checking what volume
        // the particle is in.
};

// Inline function - const avoids dynamic memory problems. Declaration here, definition of fGermaniumPV is in .cc
inline const G4VPhysicalVolume* OurDetectorConstruction::GetGermaniumPV() const{
    return fGermaniumPV;
}

#endif