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
    python compute_response.py $pname $penergy $xmin $xmax --nevents $nevents --airECAL $airECAL
done < "$input_file"
