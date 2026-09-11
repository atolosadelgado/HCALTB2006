#!/bin/bash

set -e

echo "========================================"
echo "Host: $(hostname)"
echo "OS:"
cat /etc/redhat-release
echo "========================================"

# --------------------------------------------------
# Check CVMFS
# --------------------------------------------------

echo "Checking CVMFS..."

if [ ! -d /cvmfs/sft.cern.ch ]; then
    echo "ERROR: /cvmfs/sft.cern.ch is not available"
    exit 1
fi

echo "CVMFS found:"
ls -ld /cvmfs/sft.cern.ch

# --------------------------------------------------
# Load LCG
# --------------------------------------------------

LCG_VERSION="LCG_101"
LCG_VIEW="x86_64-centos7-gcc10-opt"

LCG_SETUP="/cvmfs/sft.cern.ch/lcg/views/${LCG_VERSION}/${LCG_VIEW}/setup.sh"

if [ ! -f "${LCG_SETUP}" ]; then
    echo "ERROR: LCG setup not found:"
    echo "${LCG_SETUP}"
    exit 1
fi

echo "Loading:"
echo "  ${LCG_SETUP}"

source "${LCG_SETUP}"

echo "========================================"
echo "LCG version: ${LCG_VERSION}"
echo "LCG view:    ${LCG_VIEW}"
echo "Geant4:      $(geant4-config --version)"
echo "Compiler:    $(gcc --version | head -1)"
echo "CMake:       $(cmake --version | head -1)"
echo "========================================"

# --------------------------------------------------
# Get source
# --------------------------------------------------

echo "Cloning HCALTB2006..."

git clone -b cleanScoring \
    https://github.com/atolosadelgado/HCALTB2006.git

cd HCALTB2006

echo "Source revision:"
git rev-parse HEAD

# --------------------------------------------------
# Build
# --------------------------------------------------

echo "========================================"
echo "Building..."
echo "========================================"

cmake -S . -B build

cmake --build build -j4

# --------------------------------------------------
# Check binary
# --------------------------------------------------

if [ ! -f build/gdml_sim ]; then
    echo "ERROR: build/gdml_sim was not produced"
    exit 1
fi

echo "========================================"
echo "Build successful"
echo "========================================"

ls -lh build/gdml_sim

echo
echo "Checking dependencies:"
ldd build/gdml_sim | grep "not found" && {
    echo "ERROR: missing libraries"
    exit 1
} || true

echo
echo "Geant4 libraries:"
ldd build/gdml_sim | grep -E "Geant4|G4" || true

echo "========================================"
