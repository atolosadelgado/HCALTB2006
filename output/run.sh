#!/bin/bash

# list of energy
#energies=(2 20 100 300)
energies=(50)

# list of particles
particles=("e-" "pi-")


for p in "${particles[@]}"; do
    for E in "${energies[@]}"; do
        logfile="${p}_${E}.log"
        ../build/gdml_sim -g ../data/geometry/geometryTB2006H2.gdml -pl FTFP_BERT \
        -m ../gps_singlerun.mac -pname "$p" -penergy "$E" -nevents 5000 \
        > "$logfile" 2>&1 &
        echo "Simulation $p $E launched, log -> $logfile"
    done
done
