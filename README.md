HCAL 2006 Test Beam — Geant4 Stand-Alone Application
====================================================

This project is a stand-alone Geant4 application designed to be integrated into geant-val, the official Geant4 validation suite.
It implements the configuration of the HCAL 2006 Test Beam, as documented in: EPJ C (2009) 60: 359 

The goal of this application is to reproduce the geometry and beam conditions of the 2006 test beam setup, enabling direct comparison between simulated calorimeter response and the corresponding experimental measurements for validation purposes.

# Directory Structure

data/
Contains the detector geometry and the experimental datasets used for validation within geant-val.

include/
Header files defining Geant4 user actions and helper classes.

src/
Source files implementing the Geant4 user actions, detector construction, physics list, and other components of the simulation.
