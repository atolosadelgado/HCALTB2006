#!/bin/bash

# load LCG
source /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-el9-gcc13-opt/setup.sh

echo "LCG 105 loaded, Geant4 version 11.2.0"
echo "Running on $(hostname)"
echo "Args: $@"

# exec
./gdml_sim "$@"
