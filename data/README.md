# Data Directory

This directory contains all external inputs required by the HCAL 2006 Test Beam
stand-alone Geant4 application. These files provide the experimental reference
measurements and the detector geometry used for validation within geant-val.

## Subdirectories

### `experiment/`
Contains the experimental datasets for the 2006 HCAL test beam:
energy-response measurements for π⁻, π⁺, K⁻, K⁺, antiprotons, and protons.
These values are used in geant-val to compare the simulated calorimeter
response with the corresponding test-beam measurements.

See the `experiment/README.md` for detailed information on file contents and format.

### `geometry/`
Includes the GDML detector geometry and CMSSW configuration needed to generate
the geometry used in the validation setup. The geometry corresponds to the
2006 H2 test-beam configuration.

See the `geometry/README.md` for instructions on regenerating the GDML file.


