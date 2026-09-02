Combined CMS ECAL+HCAL test beam (2006) - Geant4 Stand-Alone Application
========================================================================

This project is a stand-alone Geant4 application designed to be integrated into geant-val, the official Geant4 validation suite.

It implements the configuration of the combined CMS ECAL+HCAL test-beam setup used in 2006, as documented in: EPJ C (2009) 60: 359

The goal of this application is to reproduce the detector geometry and beam conditions of the 2006 test beam, and to compare the simulated calorimeter response with the corresponding experimental measurements.

The benchmark provides the mean energy response, its RMS and calorimeter energy leak as a function of particle type and energy for: π+, π-, K+, K-, proton, antiproton. The application can be run with different Geant4 physics lists, allowing the test-beam data to be used as a benchmark for Geant4 hadronic physics models.

# Directory Structure
condor/
    Scripts to run the simulation in the HTCondor batch system.

data/
    Detector geometry and experimental datasets used for the validation.

include/
    Header files defining Geant4 user actions and helper classes.

src/
    Source files implementing the detector construction, physics list,
    user actions, and other components of the simulation.

plotting/
    Scripts used to calibrate the ECAL and HCAL separately, calculate
    the calorimeter response, and produce plots of the mean and RMS
    energy response.

geant-val/
    Scripts used to convert the ROOT output of the analysis into the
    JSON format required by geant-val.

The condor, data, plotting, and geant-val directories contain additional README files with more detailed information.

# Building

A Geant4 software stack must first be sourced. For example, using the LCG stack:

```
source /cvmfs/sft.cern.ch/lcg/views/LCG_106b/x86_64-el9-gcc13-opt/setup.sh
```

The application can then be compiled with CMake:
```
cmake -S . -B build
cmake --build build -- -j 4
```

The executable is created as:
```
build/gdml_sim
```

# Running the Application

The application requires a GDML geometry file and a physics-list name. A complete list of command-line options can be obtained with:

```
./build/gdml_sim -h
Usage:
  ./build/gdml_sim -g <geometry.gdml> -pl <physics_list> [options]

Required:
  -g         Geometry GDML file
  -pl        Physics list name

Optional:
  -m         Macro file
  -o         Output filename
  -penergy   Particle energy (GeV)
  -pname     Particle name (e.g. pi-)
  -airECAL   Make ECAL of air
  -nevents   Number of events (0 = macro/UI)
  -vis       Enable visualization
  -novis     Disable visualization (default)
  -visSensitiveOnly   Visualize sensitive vols only
  -enableSecondaryTrackInfo    Enable saving track information of secondary particle
                               (requires 1GB/thread of memory)
  -disableSecondaryTrackInfo   Disable saving track information of secondary particle
  -enableHistoProfileInfo      Enable saving energy and radius RMS longitudinal profiles
                               (output file size around 30MB/100kevt)
  -disableHistoProfileInfo     Disable saving energy and radius RMS longitudinal profiles
  -enableRadialCutSD           Enable radial cut when scoring energy in SensitiveDetectors
  -disableRadialCutSD          Disable radial cut when scoring energy in SensitiveDetectors
  -h         Show this help
```

An example of how to run the application:
```
./build/gdml_sim \
    -g data/geometry.gdml \
    -pl CMS \
    -pname pi- \
    -penergy 1 \
    -nevents 20000 \
    -o output.root
```

## Physics lists

If CMS is specified as the physics-list name, a dedicated physics configuration reproducing the CMSSW setup is used. This is essentially a tuned version of FTFP_BERT_EM0.

For any other physics-list name, the name is passed to the Geant4 physics-list factory.


## Optional features

### Secondary-particle track information

The `enableSecondaryTrackInfo` option enables the recording of the beginning- and end-of-track energy of secondary particles, grouped by particle type and creator process. This option requires a significant amount of memory, approximately 1 GB per thread.

### Longitudinal shower profiles

The `enableHistoProfileInfo` option enables the storage in the output TTree of longitudinal profiles of deposited energy and radial RMS. These profiles can be used to study the development of the hadronic shower. The resulting output file is approximately 30 MB per 100k events.

### Radial hit cut

The `enableRadialCutSD` option applies a radial selection to hits recorded in the sensitive detectors.

Hits occurring further than a configurable distance from the primary-particle direction are excluded from the calorimeter energy measurement.

This feature is intended to emulate the clustering algorithm used in the CMSSW reconstruction of the test-beam data.

### ECAL material replacement

The `airECAL` option replaces the ECAL materials with air.

This configuration is used for the HCAL calibration run, where the HCAL response is determined independently of the ECAL.


## Output

Output

The output ROOT filename is constructed from the main configuration parameters:
* primary particle
* kinetic energy in GeV
* number of events
* physics list
* whether the ECAL was replaced by air

For example: `HCALTB2006_e-_99.861000_2000evt_CMS_airECAL0.root`

The output file name is made as concatenation of key configuration parameters: primary particle, kinetic energy in GeV, number of events, physics list, and if ECAL was converted into air (0 means no). When the option `airECAL` is activated, the filename will end by `airECAL1`, indicating that all materials of ECAL were converted into air, so HCAL calibration is possible. An example of a file name: `HCALTB2006_e-_99.861000_2000evt_CMS_airECAL0.root`

The minimal output file contains the following objects:
```
TFile**		HCALTB2006_e-_99.861000_2000evt_CMS_airECAL0.root
 TFile*		HCALTB2006_e-_99.861000_2000evt_CMS_airECAL0.root
  KEY: TTree	tree;1	tree for HCAL 2006 TB experiment
  KEY: TH1D	hEprofile;1
  KEY: TH1D	hRprofile;1
  KEY: TDirectoryFile	inputArgs;1	inputArgs
root [2] tree->Print()
******************************************************************************
*Tree    :tree      : tree for HCAL 2006 TB experiment                       *
*Entries :     2000 : Total =          209943 bytes  File  Size =     165663 *
*        :          : Tree compression factor =   1.24                       *
******************************************************************************
*Br    0 :ECAL_eresponse : Double_t tree                                     *
*Entries :     2000 : Total  Size=      26149 bytes  File Size  =      20116 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.26     *
*............................................................................*
*Br    1 :ECAL_eresponse_raw : Double_t tree                                 *
*Entries :     2000 : Total  Size=      26225 bytes  File Size  =      20163 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.26     *
*............................................................................*
*Br    2 :HCAL_eresponse : Double_t tree                                     *
*Entries :     2000 : Total  Size=      26149 bytes  File Size  =      21405 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.19     *
*............................................................................*
*Br    3 :HCAL_eresponse_raw : Double_t tree                                 *
*Entries :     2000 : Total  Size=      26225 bytes  File Size  =      21441 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.19     *
*............................................................................*
*Br    4 :TotalEnergyECALregion : Double_t tree                              *
*Entries :     2000 : Total  Size=      26282 bytes  File Size  =      19955 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.28     *
*............................................................................*
*Br    5 :TotalEnergyHCALregion : Double_t tree                              *
*Entries :     2000 : Total  Size=      26282 bytes  File Size  =      21195 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.20     *
*............................................................................*
*Br    6 :CaloEfluxOut : Double_t tree                                       *
*Entries :     2000 : Total  Size=      26111 bytes  File Size  =      21077 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.20     *
*............................................................................*
*Br    7 :CaloEfluxIn : Double_t tree                                        *
*Entries :     2000 : Total  Size=      26092 bytes  File Size  =      18441 *
*Baskets :       16 : Basket Size=      32000 bytes  Compression=   1.38     *
*............................................................................*
```

The main branches of the tree are:

* ECAL_eresponse / HCAL_eresponse : Deposited energy in the sensitive volumes after applying the detector-specific corrections used in the CMSSW reconstruction, namely Birks quenching correction and HCAL first-layer (W0) correction

* ECAL_eresponse_raw / HCAL_eresponse_raw : Deposited energy in the sensitive volumes without the above corrections.

* TotalEnergyECALregion / TotalEnergyHCALregion : Energy deposited in the complete ECAL or HCAL region, without detector-response corrections. These quantities can be useful for studying energy that does not contribute to the reconstructed calorimeter signal, for example energy carried by nuclear fragments or other non-scoring components.

* CaloEfluxOut / CaloEfluxIn : energy carried by particles crossing the calorimeter boundary. The primary particle is excluded from these quantities. CaloEfluxIn can contain a small contribution close to the primary-particle energy when an unstable particle decays in flight. For example, a pion may decay before reaching the calorimeter.


# HTCondor Production

The condor directory contains scripts for running the calibration and production scans using HTCondor.

Two calibration scans are provided:
```
condor_submit calibrationECAL.sub
condor_submit calibrationHCAL.sub
```

Calibration consist in a number of jobs, each one shooting electrons at fix energies. The energies were chosen to be the same as for pi-.

Production scans can then be submitted for the different physics lists:
```
condor_submit production.sub
condor_submit production_FTFP_BERT.sub
condor_submit production_QGSP_INCLXX.sub
```

Production consist in a number of jobs, corresponding to the 60 experimental data points of different particle-energy, where particle are pion+, pion-, proton, antiproton, kaon+, kaon-.

## Calibration

The calibration scans shoot electrons at fixed energies.

The electron energies are chosen to be the same as the energies used in the π− production scan.

The ECAL and HCAL calibrations are performed independently. For the HCAL calibration, the ECAL is replaced by air using the airECAL option.

The experimental calorimeter calibration was performed using electrons at 50 GeV. The simulation nevertheless evaluates the response at several energies to study the energy dependence of the calibration.

## Production

The production scan contains the 60 experimental particle-energy points.

The same set of particle-energy combinations is simulated for each physics list, allowing direct comparison between physics-list configurations.

The particles included in the production scan are: π+, π-, K+, K-, proton, antiproton.

# Analysis

The analysis proceeds in two stages:

```
Geant4 simulation
       │
       ▼
   ROOT files
       │
       ├── ECAL calibration
       ├── HCAL calibration
       │
       ▼
  Energy response
       │
       ▼
    ROOT graphs
       │
       ▼
   geant-val JSON
```

All analysis scripts are located in the `plotting` directory.

The list of output files to be processed looks like:
```
HCALTB2006_anti_proton_1.271000_20000evt_CMS_airECAL0.root
HCALTB2006_e-_3.863000_2000evt_CMS_airECAL1.root
HCALTB2006_kaon+_5.527000_20000evt_CMS_airECAL0.root
HCALTB2006_pi+_5.862000_20000evt_CMS_airECAL0.root
HCALTB2006_anti_proton_2.205000_20000evt_CMS_airECAL0.root
HCALTB2006_e-_4.862000_2000evt_CMS_airECAL0.root
HCALTB2006_kaon-_5.527000_20000evt_CMS_airECAL0.root
HCALTB2006_pi-_5.862000_20000evt_CMS_airECAL0.root
...
```

## Calibration

The `calibrate.py` script processes the electron calibration scan.

For each electron energy, it determines the mean calorimeter energy and writes the result to a text file in tabular format. Once the scan has been processed, a ROOT histogram is produced from the resulting table.

For example:
```
python calibrate.py \
    --detector ECAL \
    --scan-file ../data/scan/calibration.scan \
    --nevents 2000 \
    --directoryROOTfiles ../outputFiles

python calibrate.py \
    --detector HCAL \
    --scan-file ../data/scan/calibration.scan \
    --nevents 2000 \
    --directoryROOTfiles ../outputFiles
```

The corresponding commands are also provided in `run_plot_calibration.sh`.

The calibration factors obtained from the simulation are then used to reconstruct the combined ECAL+HCAL response.

## Production response

The `compute_response.py` script calculates the mean and RMS of the combined ECAL+HCAL energy response, as well as the total energy leakage.

The script constructs an energy-response histogram by:
1. Reading the ECAL and HCAL deposited energies
2. Applying the corresponding calibration factors
3. Applying Gaussian smearing to the ECAL and HCAL response
4. Combining the ECAL and HCAL contributions
5. Fitting the resulting energy distribution

The following calibration and smearing values are taken from CMSSW 15.0.10:

```
ECAL calibration factor = 1.01
HCAL calibration factor = 106.5
ECAL smearing           = 0.362 GeV
HCAL smearing           = 0.64 GeV
```

The calibration factors may depend on the Geant4 version.

The `run_plot_reco.sh` script runs `compute_response.py` for each of the 60 particle-energy combinations.

For each particle-energy point, the analysis writes the following quantities to centroids.txt:
* particle name
* particle energy in GeV
* mean reconstructed energy in GeV
* RMS of the reconstructed energy in GeV
* particle momentum in GeV/c
* calorimeter response, defined as the mean reconstructed energy divided by the particle momentum
* energy leak in GeV

The resulting table is converted into ROOT graphs using:

```
root -q plot_centroids.cxx
```

The resulting graphs contain the quantities used to compare the simulated calorimeter response with the experimental measurements.

## Geant-val Integration

The final step is to convert the ROOT graphs produced by the analysis into the JSON format expected by geant-val.

This is done using:

```
geant-val/run_root_to_json.sh
```

The script calls `root_to_json.py` for each particle and observable.

To convert the calibration, response and energy fluctuation ROOT graphs into json files for geant-val, we can run the bash script `geant-val/run_root_to_json.sh`. Inside this bash script, a python script is called in a loop to create the corresponding graphs

For example:

```
    python root_to_json.py --input "response_${particle}.root" \
                           --graph Graph \
                           --output "response_${particle}.json" \
                           --particle "${particle}" \
                           --physics-list CMS \
                           --geant4-version "$(geant4-config --version)" \
                           --target-name "CMS ECAL+HCAL" \
                           --observable-name "Mean(f_ECAL*E_ECAL+f_HCAL*E_HCAL)"
```

The resulting JSON files contain the information required by geant-val to compare different Geant4 configurations with the experimental reference data.

The physics-list name must be explicitly provided to `root_to_json.py`.

For example, a production using FTFP_BERT should be converted with:

```
--physics-list FTFP_BERT
```

while a production using the dedicated CMSSW configuration should use:

```
--physics-list CMS
```

This ensures that the resulting validation data are correctly associated with the corresponding Geant4 physics-list configuration.

# Summary of the Validation Workflow

A complete validation campaign consists of:

1. Build the stand-alone Geant4 application
2. Run ECAL and HCAL calibration scans
3. Run the 60 particle-energy production points
4. Analyse the ROOT output
   - calorimeter calibration
   - mean response
   - RMS
   - energy leakage
5. Produce ROOT graphs
6. Convert ROOT graphs to geant-val JSON
7. Compare Geant4 predictions with
   the 2006 CMS test-beam data

This provides a reproducible stand-alone benchmark of Geant4 hadronic shower development and calorimeter response using the combined CMS ECAL+HCAL 2006 test-beam data.
