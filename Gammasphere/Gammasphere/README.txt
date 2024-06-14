# ------------------------------------------------
# Gammasphere Geant4 Simulation
# Geant4 version Name: geant4-11-02-beta-01 [MT]
#         - Greg Willmott gw00431@surrey.ac.uk
# Simulation version: 1
# ------------------------------------------------

C++ help: https://cplusplus.com/doc/tutorial/


-------------------------------
--- What is being simulated ---
-------------------------------

Geometry is a simplified version of Gammasphere, based off of the publications of I-Yang Lee. One module is placed and consists of:
    - Hevimet collimator
    - Tapered, well-shaped Ge crystal
    - BGO rear cylinder and side wedges
    - Al casing
    - Cu plate and 'electronics'

The world volume is air. The source is either based on the particle gun or the General Particle Source (GPS). Change
between these two with boolean ifGeneralSource in primaryGeneratorAction.cc. Some macros are provided to
give some options for the GPS.

The data are output in .root format through Geant's analysis manager. The energy deposited in the Germanium
is tallied on a stepping (per vertex) basis, summed per event (particle history/life) and put into a TH1D (named "EDep Hist"). 
There is an initial filter that the particle must be in Germanium (Ge) or Bismuth Germanate (BGO) to be tallied.

Gammasphere's Compton suppression is modelled by keeping track of whether or not a particle has travelled in Ge or
BGO, or both or neither on a per event basis. One step/vertex can change the booleans for the event.
    - Ge only: energy deposited goes to main spectrum
    - Ge and BGO: EDep goes to main spectrum and background spectrum (a second TH1D named "Bg EDep Hist")
    - BGO only: excluded. Tally 0. keV.
    - Neither: Tally isn't engaged. Root will put a count in the zero bin by default though.

The physics used is given in a modular way in the physics list. Different models can be swapped or turned 
off here. G4NeutronHP (High Precision) data is used. This requires the unpacking of the data in tar format found here:
https://www-nds.iaea.org/geant4/

The simulation has been tried with ENDF 8 and JEFF 3.3 data. Instructions are on the webpage for what G4 expects,
but in short after unpacking, some folders from the original G4 data need to be copied across as ENDF/JEFF is missing them.
Also global variable G4NEUTRONHPDATA needs to be set on the system to the location of the new files. I made a 'data'
folder at the top level and have written a shell script that does this copying. 

##########################################################################################
########## The number of threads is controlled by variable nThreads in main.cc. ##########
##########################################################################################

The file name of the output .root file is chosen in runAction.cc. Code is in place to output 'GSphereTest.root' or to make
a filename based on the system time. Uncomment the one you want.

Code is based on examples basic/B1 and basic/B4/B4a and previous sims of mine.

--------------------------
--- Build instructions ---
--------------------------

This README will be in the top level of the source code folder, along the folders containing
the source (.cc's) and headers (.hh's). Your build folder is expected to be made alongside, not in, this top level folder.
EXPECTED FILE STRUCTURE:
|- build
|   |- executable is built here
|   |- all original macros will be copied here by CMake
|   |- *.root outputs end up here
|- GretinaLite
|   |- include
|   |   |- *.hh files
|   |- src
|   |   |- *.cc files
|   |   | - main.cc
|   |- all macros *.mac


COMMANDS:
To configure
cmake ../Gammasphere

To make the executable
make -j4

Execute
./Gammasphere


----------------------
--- How to execute ---
----------------------

./Gammasphere       - brings up GUI. In the GUI command bar do /control/execute runMacroVis.mac to use the macro
                  and this will also execute a General Particle Source (GPS) macro within it. To change the source,
                  change which GPS macro runMacroVis.mac executes. Follow up with /control/beamOn 20 to see some
                  particle histories. This will produce a root file so be wary it may overwrite a previous run.

./Gammasphere name-of-macro.mac             - executes in batch mode (no GUI so quicker) according to instructions given in macro.
                                              The macro runMacroBatch.mac is what I've provided. 50 million histories on 10 threads
                                              with the neutron and radioactive cobalt source takes around an hour.


------------------------
--- Macros available ---
------------------------

These will need to be manually copy pasted into your build folder.

init_vis.mac - visualisation options, copied from example B1
vis.mac - more visualisation options, executed by init_vis.mac

runMacro1.mac - example run that executes another macro containing General Particle Source commands and shoots 1 particle

gps_radioactive.mac - example GPS. Radioactively decaying 60Co placed at (0,0,0). Decay products will come off isotropically.
gps_beam.mac - 1500 keV gamma beam that shoots along Z. Circular cross section, with a sigma in radius 1 +- 0.2mm and angle +-0.5deg.
gps_point_source.mac - 1500 keV gamma isotropic point source.
gps_neutron.mac - 1500 keV neutron beam source aimed at the crystal.
gps_neutrons_and_gammas.mac - 1% intensity is gps_neutron, 99% intensity is gps_radioactive.
gps_16N.mac - Nitrogen 16 radioactive ion source.

For more GPS examples this link is recommended: https://hurel.hanyang.ac.kr/Geant4/Geant4_GPS/reat.space.qinetiq.com/gps/examples/examples.html
