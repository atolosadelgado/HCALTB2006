#!/bin/bash

# load LCG
source /cvmfs/sft.cern.ch/lcg/views/LCG_106b/x86_64-el9-gcc13-opt/setup.sh

echo "LCG version: ${LCG_VERSION}"
echo "Geant4 version $(geant4-config --version)"
echo "Running on $(hostname)"
echo "Args: $@"

# exec
./gdml_sim "$@"
