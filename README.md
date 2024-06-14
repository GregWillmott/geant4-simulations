##### Geant4 simulations #####
Simulations included:
    GeGamma
        ### If you're new use this one as an example to learn Geant4 ###
        Germanium Gamma Spec. One germanium crystal as the detector analogue, encased in Aluminium, with a choice of macros for the General Particle Source. 
        60Co was used most often. Energy deposited is scored on the germanium.
    Gammasphere
        Entire array (up to 110 High Purity Germanium modules) or just one module. Germanium and bismuth germanate (BGO) volumes are treated as one monolithic detector respectively.
    GretinaLite
        Gretina array. Simulation is designed to be simpler than the UCGretina simulation that is already widely used.
    RecoilShadow
        Proof of concept for using a magnetic field and a silicon XY segmented DSSD for counting the electrons emitted from the target, with geometry designed to separate out deltas from Auger.
        Does not yet model the produced nucleus yet.
    Micron S3
        Model of ANL experiment 1977_Henderson (December 2023) 200,202Hg Coul ex. Two optional targets plus a Micron S3 style DSSD.

See images for an idea of the geometry of each simulation.

Each simulation contains it own README with compilation and execution instructions.
