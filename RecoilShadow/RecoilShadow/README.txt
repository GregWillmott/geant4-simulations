# ------------------------------------------------
# Recoil Shadow Method Geant4 Simulation
# Geant4 version Name: geant4-11-02-beta-01 [MT]
#         - Greg Willmott gw00431@surrey.ac.uk
          - Jacob Heery j.heery@surrey...
# Simulation version: 1
# ------------------------------------------------

C++ help: https://cplusplus.com/doc/tutorial/
Good website but an adblock is necessary on it now :(

-------------------------------
--- What is being simulated ---
-------------------------------
    Target: particle origin in middle        Global origin at end of beam tube         
                    \/
  ========================= \   /e- (Auger or IC)
                    ||   ____\ /
                    ||<-x->   o ___ 254Nb
  ========================= /   \
        Beam tube        || |e-   ||e-
                         || |     ||
                         || | !   || Arm 1
        vacuum           || | !B  ||
                         || | \/  ||
                            |
                         ===*DSSD=== XY pixels, like a Micron BB series Si detector but made up

https://www.micronsemiconductor.co.uk/product/

This simulation is for exploring the potential effectiveness of the recoil shadow method for detecting Internal Conversion (IC) and
Auger electrons from excited nuclei and avoiding delta electrons produced by the reaction in the target.

*** Dimensions are made up, should be realistic but check them later ***

The world volume is vacuum (G4_Galactic). The source is either based on the particle gun or the General Particle Source (GPS). Change
between these two with boolean ifGeneralSource in primaryGeneratorAction.cc. Some macros are provided to
give some options for the GPS.

Materials: 'Geant4 Book for application developers' look in appendix

The data are output in .root format through Geant's analysis manager.
Tallies/scores on the Si DSSD:
    - N(electrons) per event
    - N(all particles) per event
    - Energy deposited in Si spectrum
    - Hit pattern in Si pixels
Also count the number of IC processes.

The physics used is given in a modular way in the physics list. Different models can be swapped or turned 
off here.

##########################################################################################
########## The number of threads is controlled by variable nThreads in main.cc. ##########
##########################################################################################

The magnetic field in the arm is made at the bottom of detectorConstruction.cc

The file name of the output .root file is chosen in runAction.cc. Code is in place to output 'Test.root' or to make
a filename based on the system time. Uncomment the one you want.

In the main.cc you can choose to have a fixed seed (for reproducibility), or a random one based on system time (for truly random results).

Code is based on examples basic/B1 and basic/B4/B4a and previous sims of mine.

--------------------------
--- Build instructions ---
--------------------------

This README will be in the top level of the source code folder, along the folders containing
the source (.cc's) and headers (.hh's).
EXPECTED FILE STRUCTURE:
RecoilShadow
|- build
|   |- executable is built here
|   |- all original macros will be copied here by CMake
|   |- *.root outputs end up here
|- RecoilShadow
|   |- include
|   |   |- *.hh files
|   |- src
|   |   |- *.cc files
|   |   | - main.cc
|   |- all macros *.mac


COMMANDS:
To configure
cmake ../RecoilShadow

To make the executable
make -j4

To execute:
./RecoilShadow
./RecoilShadow runMacroBatch.mac

----------------------
--- How to execute ---
----------------------

./RecoilShadow       - brings up GUI. In the GUI command bar do /control/execute runMacroVis.mac to use the macro
                  and this will also execute a General Particle Source (GPS) macro within it. To change the source,
                  change which GPS macro which runMacroVis.mac executes. Follow up with /control/beamOn 20 to see some
                  particle histories. This will produce a root file so be wary as it may overwrite a previous run.

./RecoilShadow name-of-macro.mac          - executes in batch mode (no GUI so quicker) according to instructions given in macro.
                                            The macro runMacroBatch.mac is what I've provided.


------------------------
--- Macros available ---
------------------------

Copies are in the main source folder, and these will be copied into the build folder during compilation.
The versions in the build folder will be the ones used by the program.

init_vis.mac - visualisation options, copied from example B1
vis.mac - more visualisation options, executed by init_vis.mac

runMacroVis.mac - example run that executes another macro containing General Particle Source commands and shoots 20 particles. Designed
                  to be run within the GUI.
runMacroBatch.mac - the main method of running this for your final results.

gps_radioactive.mac - example GPS. Radioactively decaying 60Co placed at (0,0,0). Decay products will come off isotropically.

For more GPS examples this link is recommended: https://hurel.hanyang.ac.kr/Geant4/Geant4_GPS/reat.space.qinetiq.com/gps/examples/examples.html
