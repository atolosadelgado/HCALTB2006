#!/bin/bash

input_file="particle_energy.scan"
nevents=20000
airECAL=0

while read -r pname penergy xmin xmax; do
    # skip empty lines
    [[ -z "$pname" ]] && continue
    
    # skip lines starting with '#'
    [[ "$pname" =~ ^# ]] && continue

    # compute calorimeter response
    python compute_response.py  $pname $penergy $xmin $xmax \
                                --nevents $nevents          \
                                --airECAL $airECAL          \
                                --ECAL-calibrationFactor 1.01  \
                                --HCAL-calibrationFactor 106.5 \
                                --ECAL-smearing 0.362 \
                                --HCAL-smearing 0.64
done < "$input_file"
