# ------------------------------------------------
# Germanium Gamma Spec Geant4 Simulation
# Geant4 version Name: geant4-11-02-beta-01 [MT]
#         - Greg Willmott gw00431@surrey.ac.uk
# Simulation version: 2
# ------------------------------------------------

C++ help: https://cplusplus.com/doc/tutorial/
They've covered this website in ads so use an adblock extension to your browser


--- What is being simulated ---
Geometry is a simple analogue of a Germanium detector, with a cylindrical active volume, and aluminium casing.
The world volume is air. The source is either a 1173 keV gamma fired from (0,0,0) at (0,0,1) if the particle
gun is used, otherwise the General Particle Source uses the macro which the user provides at run time. Change
between these two with boolean ifGeneralSource in primaryGeneratorAction.cc. Some macros are provided to
give some options for the GPS.

The data are output in .root format through Geant's analysis manager. The energy deposited in the Germanium
is tallied on a stepping (per vertex) basis, summed per event (particle history/life) and put into a TH1D
histogram and a nTuple.

The physics used is given in a modular way in the physics list. Different models can be swapped or turned 
off here.

########## The number of threads is controlled by variable nThreads in GeGammaMain.cc. ##########

The file name of the output .root file is chosen in runAction.cc. Code is in place to output 'GeGammaTest.root' or to make
a filename based on the system time. Uncomment the one you want.

Code is based on examples basic/B1 and basic/B4/B4a and previous sims of mine.


--- Build instructions ---
This README will be in the top level of the source code folder, along with the folders containing
the source (.cc's) and headers (.hh's). Your build folder is expected to be made alongside, not in, this top level folder.
EXPECTED FILE STRUCTURE:
|- build
|   |- executable is built here
|   |- all macros need will be copied here too
|   |- *.root outputs end up here
|- GeGamma
|   |- include
|   |   |- *.hh files
|   |- src
|   |   |- *.cc files
|   |   | - main.cc
|   |- all macros *.mac


COMMANDS:
To configure
cmake ../GeGamma

To make/compile the executable
make -j4

Execute
./GeGamma


--- EXECUTION OPTIONS ---
./GeGamma       - brings up GUI. In the GUI command bar do /control/execute runMacroVis.mac to use the macro
                  and this will also execute a General Particle Source (GPS) macro within it. To change source,
                  change which GPS macro runMacroVis.mac executes. Follow up with /control/beamOn 20 to see some
                  particle histories.
./GeGamma name-of-macro.mac    - executes in batch (terminal only) mode (no GUI so quicker) according to instructions given in macro.
                                 If you're using my macros, do ./GeGamma runMacroVis.mac



--- Macros available ---

init_vis.mac - visualisation options, copied from example B1
vis.mac - more visualisation options, executed by init_vis.mac

runMacroVis.mac - complete example run that executes another macro containing General Particle Source commands and shoots 1 particle, designed
                  to be run within the visualiser through /control/execute runMacroVis.mac
runMacroBatch.mac - complete example run like above, but runs 1,000,000 particles rather than 1, hence is unsuitable for the visualiser



--- Different particle sources for runMacroVis/Batch.mac ---

gps_radioactive.mac - example GPS. Radioactively decaying 60Co placed at (0,0,0). Decay products will come off isotropically.
gps_beam.mac - 1500 keV gamma beam that shoots along Z. Circular cross section, with a sigma in radius 1 +- 0.2mm and angle +-0.5deg.
gps_point_source.mac - 1500 keV gamma isotropic point source.
gps_neutron.mac - 1500 keV neutron beam source aimed at the crystal.
gps_neutrons_and_gammas.mac - 1% intensity is gps_neutron, 99% intensity is gps_radioactive.

For more GPS examples this link is recommended: https://hurel.hanyang.ac.kr/Geant4/Geant4_GPS/reat.space.qinetiq.com/gps/examples/examples.html
