#!/bin/bash

set -e

python calibrate.py --detector ECAL --scan-file ../data/scan/calibration.scan --nevents 2000 --directoryROOTfiles ../outputFiles
python calibrate.py --detector HCAL --scan-file ../data/scan/calibration.scan --nevents 2000 --directoryROOTfiles ../outputFiles
