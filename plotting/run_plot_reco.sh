#!/bin/bash

input_file="particle_energy.scan"
nevents=80000
airECAL=0

# remove old file if exist
rm centroids.txt

# dump into a text file `centroids.txt` the calorimeter response and energy fluctuation
while read -r pname penergy xmin xmax; do
    # skip empty lines
    [[ -z "$pname" ]] && continue
    
    # skip lines starting with '#'
    [[ "$pname" =~ ^# ]] && continue

    # compute calorimeter response
    python compute_response.py  $pname $penergy $xmin $xmax \
                                --nevents $nevents          \
                                --airECAL $airECAL          \
                                --ECAL-calibrationFactor 1.022  \
                                --HCAL-calibrationFactor 110.9 \
                                --ECAL-smearing 0.362 \
                                --HCAL-smearing 0.64 \
                                --directoryROOTfiles ../../v10.7.2/root/FTFP_BERT \
                                --physics-list FTFP_BERT

done < "$input_file"

# make ROOT graphs of the calorimeter response and energy fluctuation
root -q plot_centroids.cxx
