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
        OurDetectorConstruction() = default; // Constr
        ~OurDetectorConstruction() override = default; // Destr
        G4VPhysicalVolume* Construct() override; // Function from Geant

        // Extra member, declaration here, but definition at bottom of file
        const G4VPhysicalVolume* GetSiliconPV() const;
        const G4VPhysicalVolume* GetLeadPV() const;
    private:
        G4VPhysicalVolume* fSiliconPV = nullptr; // Pointer which will point to the Silicon Phys Volume.
        // Purpose is to be sent to be available in the stepping action when checking what volume
        // the particle is in.
        G4VPhysicalVolume* fLeadPV = nullptr; // for beam stop
};

// Inline function - const avoids dynamic memory problems. Declaration here, definition of fSiliconPV is in .cc
inline const G4VPhysicalVolume* OurDetectorConstruction::GetSiliconPV() const{
    return fSiliconPV;
}

inline const G4VPhysicalVolume* OurDetectorConstruction::GetLeadPV() const{
    return fLeadPV;
}

#endif