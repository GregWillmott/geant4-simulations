##########################################################
#             ISOLDE SOLENOIDAL SPECTROMETER             #
#                   Geant4 simulation                    #
##########################################################
University of Surrey
Authors:
	Jack Henderson (jack.henderson@surrey.ac.uk) - primary particle generation
	Greg Willmott (gw00431@surrey.ac.uk) - geometry, histograms
	
AIMS: to simulate the transport of (d,p) reaction products within the solenoidal magnetic field
      and to estimate the response in the Liverpool silicon array.
      
PREREQUISITES: Root (v6 onwards should be fine (6.31 used))
	       Geant4.11.02 (initially made for 4.10.XX but G4MultiUnion was bugged)

COMPILATION: Create a 'build' folder alongside the src folder
	     cmake ../src
	     make

EXECUTION: ./runISS
	  this will bring up the GUI
	  ./runISS <name-of-your-macro>
	  this will run the simulation in batch mode. Use run.mac
	  
IMPORTANT: before running one particle, the macro run.mac must be executed.
This sets the parameters for the kinematics, and points the code to where
the angular distribution file is. Do this with
/control/execute run.mac

The angular distribution file uses '#' as a comment marker, as per typical macros
and has cross section in mb/steradian for every single degree between 0 and 180.

One file can contain the contribution of one state's population so multiple simulations
are needed to model an experiment.

The (d,p) kinematics were written as part of the FAUST project.

GEOMETRY: this is modelled internally using the CSV volumes (G4Box, G4Tubs etc). The silicon
          p and n side strips are modelled separately beneath a dead layer of 0.00047mm. Each p
          strip has a unique copy number (same for n) that can be found in the attached .ods
          
HISTOGRAMS: (unfinished) the aim is to produce an output that is compatable with ISS Sort to
            aid comparison to experimental data.

REFERENCES: https://isolde-solenoidal-spectrometer.web.cern.ch/ (public)
            https://twiki.cern.ch/twiki/bin/view/ISS/WebHome    (private)
            Measurements taken in person at ISOLDE
            
A copy of my (Greg's) notes are available upon request which go over geometry creation
