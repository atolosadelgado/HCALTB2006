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

Branches:
* master: trial to make a simple (just scoring energy) Geant4 stand alone application to reproduce the results in the paper. It did not work. Project forked at commit ac5d8d72b37eca2fa0145743f3073476ec94f6b4 to the branch below
* cmssw_actions: branch aiming to migrate the CMSSW actions for scoring energy and produce the mean energy plots as in the original paper

In cmssw_actions branch, I am reimplementing SD actions used by CMSSW steering file [(link)](https://github.com/cms-sw/cmssw/blob/70edd4697f3e2203264aafdd80be69005c50825a/SimG4CMS/HcalTestBeam/test/python/run_tb06_all_cfg.py#L138):
* EcalSensitiveDetector, HcalSensitiveDetector: I have gather the 4 classess in CMSSW (EcalSD -> CaloSD -> SensitiveCaloDetector -> SensitiveDetector -> G4VSensitiveDetector) into SimpleCaloSD class. At the moment only scores energy, without corrections. Next step is to implement the corrections.
* HcalTB06BeamDetector, EcalTBH4BeamDetector: not clear if needed later, to be investigated
* CaloTrkProcessing: does not seem needed
* HcalTB02SensitiveDetector
