#!/bin/bash

set -e

python calibrate.py --detector ECAL --scan-file ../data/scan/calibration.scan --nevents 2000 --directoryROOTfiles ../../v10.7.2/root/calibration/
python calibrate.py --detector HCAL --scan-file ../data/scan/calibration.scan --nevents 2000 --directoryROOTfiles ../../v10.7.2/root/calibration/
