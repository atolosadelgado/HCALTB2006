HCAL 2006 Test Beam — Geant4 Stand-Alone Application
====================================================

This project is a stand-alone Geant4 application designed to be integrated into geant-val, the official Geant4 validation suite.
It implements the configuration of the HCAL 2006 Test Beam, as documented in: EPJ C (2009) 60: 359 

The goal of this application is to reproduce the geometry and beam conditions of the 2006 test beam setup, enabling direct comparison between simulated calorimeter response and the corresponding experimental measurements for validation purposes.

# Directory Structure

data/
Contains the detector geometry and the experimental datasets used for validation within geant-val.

include/
Header files defining Geant4 user actions and helper classes.

src/
Source files implementing the Geant4 user actions, detector construction, physics list, and other components of the simulation.

Branches:
* master: trial to make a simple (just scoring energy) Geant4 stand alone application to reproduce the results in the paper. It did not work. Project forked at commit ac5d8d72b37eca2fa0145743f3073476ec94f6b4 to the branch below
* cmssw_actions: branch aiming to migrate the CMSSW actions for scoring energy and produce the mean energy plots as in the original paper

In cmssw_actions branch, I am reimplementing SD actions used by CMSSW steering file [(link)](https://github.com/cms-sw/cmssw/blob/70edd4697f3e2203264aafdd80be69005c50825a/SimG4CMS/HcalTestBeam/test/python/run_tb06_all_cfg.py#L138):
* EcalSensitiveDetector, HcalSensitiveDetector: I have gather the 4 classess in CMSSW (EcalSD -> CaloSD -> SensitiveCaloDetector -> SensitiveDetector -> G4VSensitiveDetector) into SimpleCaloSD class. At the moment only scores energy, without corrections. Next step is to implement the corrections.
* HcalTB06BeamDetector, EcalTBH4BeamDetector: not clear if needed later, to be investigated
* CaloTrkProcessing: does not seem needed
* HcalTB02SensitiveDetector

* By printouts:

adding this in each cc, then invoke REGISTER_FCN; in each method
#define REGISTER_FCN { \
    std::ofstream ofile("mydebug.txt", std::ios::app); \
    ofile << __PRETTY_FUNCTION__ << std::endl; \
    ofile.close(); \
}

sort mydebug.txt | uniq > mydebug_uniq_hcal.txt

SensitiveDetector::SensitiveDetector(const std::string&, const SensitiveDetectorCatalog&, bool, const std::string&)

virtual G4bool CaloSD::ProcessHits(G4Step*, G4TouchableHistory*)

double ECalSD::curve_LY(const G4LogicalVolume*)
double ECalSD::getBirkL3(const G4Step*)
virtual double ECalSD::getEnergyDeposit(const G4Step*)
virtual int ECalSD::getTrackID(const G4Track*)
virtual uint16_t ECalSD::getDepth(const G4Step*)
virtual uint32_t ECalSD::setDetUnitId(const G4Step*)
void ECalSD::getBaseNumber(const G4Step*)
void ECalSD::initMap()
void ECalSD::setNumberingScheme(EcalNumberingScheme*)

HCalSD::HCalSD(const std::string&, const HcalDDDSimConstants*, const HcalDDDRecConstants*, const HcalSimulationConstants*, const HBHEDarkening*, const HBHEDarkening*, const SensitiveDetectorCatalog&, const edm::ParameterSet&, const SimTrackManager*)
bool HCalSD::isItConicalBundle(const G4LogicalVolume*)
bool HCalSD::isItHF(const G4Step*)
bool HCalSD::isItPMT(const G4LogicalVolume*)
bool HCalSD::isItScintillator(const G4Material*)
bool HCalSD::isItStraightBundle(const G4LogicalVolume*)
uint32_t HCalSD::setDetUnitId(HcalNumberingFromDDD::HcalID&)
uint32_t HCalSD::setDetUnitId(int, const G4ThreeVector&, int, int)
virtual bool HCalSD::filterHit(CaloG4Hit*, double)
virtual bool HCalSD::getFromLibrary(const G4Step*)
virtual double HCalSD::getEnergyDeposit(const G4Step*)
virtual uint32_t HCalSD::setDetUnitId(const G4Step*)
virtual void HCalSD::endEvent()
virtual void HCalSD::initEvent(const BeginOfEvent*)
void HCalSD::modifyDepth(HcalNumberingFromDDD::HcalID&)
void HCalSD::setNumberingScheme(HcalNumberingScheme*)

HcalTB06BeamSD::HcalTB06BeamSD(const std::string&, const HcalTB06BeamParameters*, const SensitiveDetectorCatalog&, const edm::ParameterSet&, const SimTrackManager*)

* By igprof + ProcessHits

igprof -d -pp -t cmsRun -o igprof.pp cmsRun CMSSW_15_0_10/FTFP_BERT_p_RR_.py -n 1
igprof-analyse -g -d igprof.pp > callgraph.txt
# take the class::method substring from each line, remove the ones that start with G4, and look for unique strings
grep -oP '\w+::\w+(?=\()' callgraph.txt | grep -v '^G4' | sort | uniq > mydebug_uniq_hcal.txt

ECalSD::getBaseNumber
ECalSD::getBirkL3
ECalSD::getDepth
ECalSD::getEnergyDeposit
ECalSD::setDetUnitId
EcalBarrelNumberingScheme::getUnitID
EcalBaseNumber::addLevel
EcalBaseNumber::getCopyNumber

HCalSD::filterHit
HCalSD::getEnergyDeposit
HCalSD::getFromLibrary
HCalSD::isItConicalBundle
HCalSD::isItHF
HCalSD::isItPMT
HCalSD::isItScintillator
HCalSD::isItStraightBundle
HCalSD::modifyDepth
HCalSD::setDetUnitId

CaloSD::ProcessHits
CaloSD::checkHit
CaloSD::cleanHitCollection
CaloSD::createNewHit
CaloSD::getTrackID
CaloSD::hitExists
CaloSD::saveHit
CaloSD::update

HcalDDDSimConstants::findDepth
HcalDDDSimConstants::getEtaDepth
HcalDDDSimConstants::layerGroup
HcalNumberingFromDDD::unitID
HcalTB02ParametersESModule::produce
HcalTB02ParametersFromDD::build
HcalTB06Analysis::HcalTB06Analysis
HcalTB06Histo::HcalTB06Histo

cd work/CMSSW_HCAL2006/
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=el9_amd64_gcc12
cmsrel CMSSW_15_0_10
cd CMSSW_15_0_10/src/
cmsenv
export SITECONFIG_PATH=/cvmfs/cms.cern.ch/SITECONF/T0_CH_CERN


Logical Volumes attached to HCal SD,
HBScintillatorLayer0In1
HBScintillatorLayer0In2
HBScintillatorLayer1In1
HBScintillatorLayer1In2
HBScintillatorLayer2In1
HBScintillatorLayer2In2
HBScintillatorLayer3In1
HBScintillatorLayer3In2
HBScintillatorLayer4In1
HBScintillatorLayer4In2
HBScintillatorLayer5In1
HBScintillatorLayer5In2
HBScintillatorLayer6In1
HBScintillatorLayer6In2
HBScintillatorLayer7In1
HBScintillatorLayer7In2
HBScintillatorLayer8In1
HBScintillatorLayer8In2
HBScintillatorLayer9In1
HBScintillatorLayer9In2
HBScintillatorLayer10In1
HBScintillatorLayer10In2
HBScintillatorLayer11In1
HBScintillatorLayer11In2
HBScintillatorLayer12In1
HBScintillatorLayer12In2
HBScintillatorLayer13In1
HBScintillatorLayer13In2
HBScintillatorLayer14In1
HBScintillatorLayer14In2
HBScintillatorLayer15In1
HBScintillatorLayer15In2
HBScintillatorLayer16In1
HBScintillatorLayer16In2
HEScintillatorPart3Layer02Phi0
HEScintillatorPart3Layer02Phi1
HEScintillatorPart3Layer03Phi0
HEScintillatorPart3Layer03Phi1
HEScintillatorPart3Layer04Phi0
HEScintillatorPart3Layer04Phi1
HEScintillatorPart3Layer05Phi0
HEScintillatorPart3Layer05Phi1
HEScintillatorPart3Layer06Phi0
HEScintillatorPart3Layer06Phi1
HEScintillatorPart4Layer07Phi0
HEScintillatorPart4Layer07Phi1
HEScintillatorPart4Layer08Phi0
HEScintillatorPart4Layer08Phi1
HEScintillatorPart4Layer09Phi0
HEScintillatorPart4Layer09Phi1
HEScintillatorPart4Layer10Phi0
HEScintillatorPart4Layer10Phi1
HEScintillatorPart4Layer11Phi0
HEScintillatorPart4Layer11Phi1
HEScintillatorPart4Layer12Phi0
HEScintillatorPart4Layer12Phi1
HEScintillatorPart5Layer13Phi0
HEScintillatorPart5Layer13Phi1
HEScintillatorPart5Layer14Phi0
HEScintillatorPart5Layer14Phi1
HEScintillatorPart5Layer15Phi0
HEScintillatorPart5Layer15Phi1
HEScintillatorPart5Layer16Phi0
HEScintillatorPart5Layer16Phi1
HEScintillatorPart5Layer17Phi0
HEScintillatorPart5Layer17Phi1
HEScintillatorPart5Layer18Phi0
HEScintillatorPart5Layer18Phi1
HTSensLayer0
HTSensLayer1
HTSensLayer2
HTSensLayer3

LV attached to ECAL,
EBRY_01
EBRY_01_refl
EBRY_02
EBRY_02_refl
EBRY_03
EBRY_03_refl
EBRY_04
EBRY_04_refl
EBRY_05
EBRY_05_refl
EBRY_06
EBRY_06_refl
EBRY_07
EBRY_07_refl
EBRY_08
EBRY_08_refl
EBRY_09
EBRY_09_refl
EBRY_10
EBRY_10_refl
EBRY_11
EBRY_11_refl
EBRY_12
EBRY_12_refl
EBRY_13
EBRY_13_refl
EBRY_14
EBRY_14_refl
EBRY_15
EBRY_15_refl
EBRY_16
EBRY_16_refl
EBRY_17
EBRY_17_refl
