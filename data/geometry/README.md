# Geometry Files

This directory contains the geometry description and configuration utilities
for generating the GDML file used in the HCAL 2006 Test Beam simulation.

## Files

- **geometryTB2006H2.gdml**  
  GDML geometry corresponding to the 2006 H2 test-beam setup.

- **run2006_cfg.py**  
  CMSSW configuration file used to extract or regenerate the GDML geometry.

## Generating the GDML File

To extract the geometry GDML file, run `cmsRun run2006_cfg.py` 

This command produces `geometryTB2006H2.gdml` using the geometry definitions
from the CMSSW configuration.

## Reference

Geometry and test-beam configuration follow the description in EPJ C (2009) 60: 359
