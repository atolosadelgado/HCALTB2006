#!/bin/bash

set -e

python calibrate.py --detector ECAL --scan-file calibration.scan --nevents 2000
python calibrate.py --detector HCAL --scan-file calibration.scan --nevents 2000
