#!/bin/bash

set -e

# LCG environment used to compile gdml_sim
LCG_VERSION="LCG_101"
LCG_VIEW="x86_64-centos7-gcc10-opt"
LCG_SETUP="/cvmfs/sft.cern.ch/lcg/views/${LCG_VERSION}/${LCG_VIEW}/setup.sh"

echo "========================================"
echo "Host: $(hostname)"
echo "OS:"
cat /etc/redhat-release
echo "========================================"

echo "Loading LCG environment:"
echo "  ${LCG_SETUP}"

if [ ! -f "${LCG_SETUP}" ]; then
    echo "ERROR: LCG setup script not found:"
    echo "  ${LCG_SETUP}"
    exit 1
fi

source "${LCG_SETUP}"

echo "========================================"
echo "LCG version:  ${LCG_VERSION}"
echo "LCG view:     ${LCG_VIEW}"
echo "Geant4:       $(geant4-config --version)"
echo "geant4-config: $(which geant4-config)"
echo "Host:         $(hostname)"
echo "Args:         $@"
echo "========================================"

echo "Running gdml_sim..."

exec ./gdml_sim "$@"
