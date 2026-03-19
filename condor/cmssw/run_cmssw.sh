#!/bin/bash
# run_cmssw.sh
set -e
set -x

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=el9_amd64_gcc12
cmsrel CMSSW_15_0_10

export SITECONFIG_PATH=/cvmfs/cms.cern.ch/SITECONF/T0_CH_CERN
export USER_CXXFLAGS="-Wno-error=missing-braces"

WORKDIR="${_CONDOR_SCRATCH_DIR:-$PWD}"
CMSSW_PATH="/afs/cern.ch/user/a/atolosad/work/work/HCALTB_cleanScoring/CMSSW_15_0_10"
pushd "$CMSSW_PATH/src" > /dev/null

# setup CMSSW enviromental variables within the session
eval `scram runtime -sh`
popd > /dev/null

cd "$WORKDIR"
cmsRun "$@"
