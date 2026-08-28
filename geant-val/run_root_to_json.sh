#!/bin/bash

set -e

python root_to_json.py --input HCAL_calibration.root \
                                    --graph Graph \
                                    --output HCAL_calibration.json \
                                    --particle e- \
                                    --physics-list CMS \
                                    --geant4-version "$(geant4-config --version)" \
                                    --target-name "CMS HCAL"  \
                                    --observable-name "f_HCAL"

python root_to_json.py --input ECAL_calibration.root \
                                    --graph Graph \
                                    --output ECAL_calibration.json \
                                    --particle e- \
                                    --physics-list CMS \
                                    --geant4-version "$(geant4-config --version)" \
                                    --target-name "CMS ECAL" \
                                    --observable-name "f_ECAL"



particles=("pi+" "pi-" "proton" "anti_proton" "kaon+" "kaon-")

for particle in "${particles[@]}"; do

    echo "========================================"
    echo "Processing ${particle}"
    echo "========================================"

    python root_to_json.py --input "response_${particle}.root" \
                           --graph Graph \
                           --output "response_${particle}.json" \
                           --particle "${particle}" \
                           --physics-list CMS \
                           --geant4-version "$(geant4-config --version)" \
                           --target-name "CMS ECAL+HCAL" \
                           --observable-name "Mean(f_ECAL*E_ECAL+f_HCAL*E_HCAL)"

    python root_to_json.py --input "sigma_${particle}.root" \
                           --graph Graph \
                           --output "sigma_${particle}.json" \
                           --particle "${particle}" \
                           --physics-list CMS \
                           --geant4-version "$(geant4-config --version)" \
                           --target-name "CMS ECAL+HCAL" \
                           --observable-name "RMS(f_ECAL*E_ECAL+f_HCAL*E_HCAL)"

done

echo "All particles processed successfully."
