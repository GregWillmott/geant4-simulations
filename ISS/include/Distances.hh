// Spare file for keeping some doubles

#ifndef Distances_h
#define Distances_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

// Edit these as needed. Must be const as they're used in the DetectorConstruction and EventAction
const G4double end_d = 15.2*mm; // end cap of array depth
const G4double target_to_array_end_cap = 295.*mm; // target centre to end cap in Z
const G4double asic_to_asic_gap = 5.*mm; // self explanatory
const G4double end_cap_to_strip = 6.75*mm; // z distance end of the hexagon to the start of the first p strip
const G4double pSide_d = 0.95*mm; // z length of a p-side strip

#endif