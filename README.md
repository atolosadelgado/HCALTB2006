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

