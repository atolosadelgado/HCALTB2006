#!/bin/bash

input_file="particle_energy.scan"
nevents=20000
airECAL=0

while read -r pname penergy xmin xmax; do
    # skip empty lines
    [[ -z "$pname" ]] && continue
    
    # skip lines starting with '#'
    [[ "$pname" =~ ^# ]] && continue

    # run root
    #root -l -b -q "plot_reco.cxx(\"$pname\", $penergy, $xmin, $xmax, $nevents, $airECAL)"
    python plot_reco.py $pname $penergy $xmin $xmax --nevents $nevents --airECAL $airECAL
done < "$input_file"
