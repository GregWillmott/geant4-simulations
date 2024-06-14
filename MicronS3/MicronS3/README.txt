# ------------------------------------------------
# Micron S3 DSSD Geant4 Simulation
# Geant4 version Name: geant4-11-02-beta-01 [MT]
#         - Greg Willmott gw00431@surrey.ac.uk
# Simulation version: 1
# ------------------------------------------------

C++ help: https://cplusplus.com/doc/tutorial/

-------------------------------
--- What is being simulated ---
-------------------------------

Geometry is a Micron S3 DSSD with the following volumes:
    - Front & back layers of plastic (PVC)
    - Centre layer of FR4 circuit board material
    - Si pixels (not monolithic, each pixel is separate)
    - Target from Argonne Dec experiment (120Sn or natTi)

https://www.micronsemiconductor.co.uk/product/s3/

*** I didn't have access to any technical drawings or specifications, so material definitions and geometies should be checked. ***

The world volume is vacuum (G4_Galactic). The source is either based on the particle gun or the General Particle Source (GPS). Change
between these two with boolean ifGeneralSource in primaryGeneratorAction.cc. Some macros are provided to
give some options for the GPS.

The data are output in .root format through Geant's analysis manager. The energy deposited in the silicon
is tallied on a stepping (per vertex) basis, summed per event (particle history/life) and put into a nTuple column. The x, y, and r, phi
of any hit is tallied too.

There is an initial filter that the particle must be in silicon to be tallied.

The physics used is given in a modular way in the physics list. Different models can be swapped or turned 
off here.

##########################################################################################
########## The number of threads is controlled by variable nThreads in main.cc. ##########
##########################################################################################

The file name of the output .root file is chosen in runAction.cc. Code is in place to output 'S3Test.root' or to make
a filename based on the system time. Uncomment the one you want.

In the main.cc you can choose to have a fixed seed (for reproducibility), or a random one based on system time (for truly random results).

Code is based on examples basic/B1 and basic/B4/B4a and previous sims of mine.

--------------------------
--- Build instructions ---
--------------------------

This README will be in the top level of the source code folder, along the folders containing
the source (.cc's) and headers (.hh's).
EXPECTED FILE STRUCTURE:
|- build
|   |- executable is built here
|   |- all original macros will be copied here by CMake
|   |- *.root outputs end up here
|- MicronS3
|   |- include
|   |   |- *.hh files
|   |- src
|   |   |- *.cc files
|   |   | - main.cc
|   |- all macros *.mac


COMMANDS:
To configure
cmake ../MicronS3

To make the executable
make -j4

To execute:
./MicronS3
./MicronS3 runMacroBatch.mac

----------------------
--- How to execute ---
----------------------

./MicronS3       - brings up GUI. In the GUI command bar do /control/execute runMacroVis.mac to use the macro
                  and this will also execute a General Particle Source (GPS) macro within it. To change the source,
                  change which GPS macro which runMacroVis.mac executes. Follow up with /control/beamOn 20 to see some
                  particle histories. This will produce a root file so be wary as it may overwrite a previous run.

./MicronS3 name-of-macro.mac             - executes in batch mode (no GUI so quicker) according to instructions given in macro.
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
gps_beam.mac - 1500 keV gamma beam that shoots along Z. Circular cross section, with a sigma in radius 1 +- 0.2mm and angle +-0.5deg.
gps_point_source.mac - 1500 keV gamma isotropic point source.
gps_neutron.mac - 1500 keV neutron beam source.
gps_neutrons_and_gammas.mac - 1% intensity is gps_neutron, 99% intensity is gps_radioactive.
gps_16N.mac - Nitrogen 16 radioactive ion source.
gps_beam_200Hg.mac - 783.7 keV 31+ charge ion beam.

For more GPS examples this link is recommended: https://hurel.hanyang.ac.kr/Geant4/Geant4_GPS/reat.space.qinetiq.com/gps/examples/examples.html
