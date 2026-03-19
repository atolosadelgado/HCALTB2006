import os

# ==========================
# INPUTS
# ==========================
pname = "pim"                 # particle name
pdgID = -211                  # PDG ID (pion minus)
#kinetic_energies = [2, 3]
kinetic_energies = [2, 3, 4, 5, 6, 7, 8, 9, 20, 30, 50, 100, 150, 200, 300]  # in GeV
mass = 0.1395701              # particle mass in GeV

template = """
import FWCore.ParameterSet.Config as cms 

from Configuration.Eras.Modifier_h2tb_cff import h2tb   

process = cms.Process("PROD", h2tb) 

process.load("SimG4CMS.HcalTestBeam.TB2006GeometryXML_cfi") 

from SimG4CMS.HcalTestBeam.TB2006Analysis_cfi import * 
process = testbeam2006(process) 

process.TFileService = cms.Service("TFileService", 
  fileName = cms.string("HCAL2006_CMSSW_FTFP_BERT_{pname}_{pkinetic}GeV.root")
) 

process.common_beam_direction_parameters.MinE = cms.double({total_energy})
process.common_beam_direction_parameters.MaxE = cms.double({total_energy})
process.common_beam_direction_parameters.PartID = cms.vint32({pdgID}) # e-, 11, pion minus: -211) 
process.generator.PGunParameters.MinE = process.common_beam_direction_parameters.MinE 
process.generator.PGunParameters.MaxE = process.common_beam_direction_parameters.MaxE 
process.generator.PGunParameters.PartID = process.common_beam_direction_parameters.PartID 
process.VtxSmeared.MinE = process.common_beam_direction_parameters.MinE 
process.VtxSmeared.MaxE = process.common_beam_direction_parameters.MaxE 
process.VtxSmeared.PartID = process.common_beam_direction_parameters.PartID 
process.testbeam.MinE = process.common_beam_direction_parameters.MinE 
process.testbeam.MaxE = process.common_beam_direction_parameters.MaxE 
process.testbeam.PartID = process.common_beam_direction_parameters.PartID 
process.testbeam.TestBeamAnalysis.EcalFactor = cms.double(1.01) 
process.testbeam.TestBeamAnalysis.HcalFactor = cms.double(106.5) 

process.maxEvents = cms.untracked.PSet( 
  input = cms.untracked.int32(20000) 
) 

process.g4SimHits.Physics.type = "SimG4Core/Physics/FTFP_BERT" 

process.g4SimHits.OnlySDs = ["CaloTrkProcessing", "EcalTBH4BeamDetector", "HcalTB02SensitiveDetector", "HcalTB06BeamDetector", "EcalSensitiveDetector", "HcalSensitiveDetector"] 

process.options = cms.untracked.PSet(
    numberOfThreads = cms.untracked.uint32(4)
    )
#process.g4SimHits.CheckGeometry = cms.untracked.bool(True)
#process.g4SimHits.G4CheckOverlap.OverlapFlag = cms.bool(False)
#process.g4SimHits.G4CheckOverlap.OutputBaseName = cms.string("TB2006H2")
#process.g4SimHits.FileNameGDML    = 'geometryTB2006H2.gdml'
process.g4SimHits.SteppingVerbosity = 0
process.g4SimHits.UseMagneticField = cms.bool(False)
process.g4SimHits.RunAction.outputFile = "HCAL2006_CMSSW_FTFP_BERT_{pname}_{pkinetic}GeV_simpleout"

# =========================================================
# Disable all secondary killing in Geant4 (CMSSW)
# Put this AFTER all process modifiers / era customizations
# =========================================================


process.g4SimHits.StackingAction.TrackNeutrino = True
process.g4SimHits.StackingAction.KillGamma     = False
process.g4SimHits.StackingAction.KillDeltaRay  = False
process.g4SimHits.StackingAction.KillHeavy     = False

# --- Disable Russian Roulette (StackingAction)
process.g4SimHits.StackingAction.RusRoGammaEnergyLimit   = 0.0
process.g4SimHits.StackingAction.RusRoNeutronEnergyLimit = 0.0
process.g4SimHits.StackingAction.RusRoProtonEnergyLimit  = 0.0

# Set ALL roulette factors to 1 (no killing)
for attr in dir(process.g4SimHits.StackingAction):
    if attr.startswith("RusRo") and attr not in [
        "RusRoGammaEnergyLimit",
        "RusRoNeutronEnergyLimit",
        "RusRoProtonEnergyLimit"
    ]:
        setattr(process.g4SimHits.StackingAction, attr, 1.0)

# --- Disable EM Russian Roulette (Physics block)
process.g4SimHits.Physics.RusRoElectronEnergyLimit = 0.0

for attr in dir(process.g4SimHits.Physics):
    if attr.startswith("RusRo") and attr != "RusRoElectronEnergyLimit":
        setattr(process.g4SimHits.Physics, attr, 1.0)

# --- Disable tracking cuts
process.g4SimHits.Physics.TrackingCut = False
process.g4SimHits.Physics.CriticalEnergyForVacuum = 0.0

# --- Minimize production cuts (important!)
process.g4SimHits.Physics.DefaultCutValue = 1e-3  # cm (muy pequeño)

# Opcional (más agresivo)
# process.g4SimHits.Physics.CutsPerRegion = False

# --- Disable time / region killing
process.g4SimHits.Physics.MaxTrackTime        = 1e9
process.g4SimHits.Physics.MaxTrackTimeForward = 1e9
process.g4SimHits.Physics.DeadRegions = []

# --- Disable heavy particle suppression
process.g4SimHits.StackingAction.NeutronThreshold = 0.0
process.g4SimHits.StackingAction.ProtonThreshold  = 0.0
process.g4SimHits.StackingAction.IonThreshold     = 0.0

# Also in CaloSD (duplicated there!)
process.g4SimHits.CaloSD.NeutronThreshold = 0.0
process.g4SimHits.CaloSD.ProtonThreshold  = 0.0
process.g4SimHits.CaloSD.IonThreshold     = 0.0

# --- Calorimeter track persistence
process.g4SimHits.CaloSD.EminTrack = 0.0
process.g4SimHits.CaloSD.SuppressHeavy = False

# --- Extra safety: stepping limits
process.g4SimHits.Physics.ElectronStepLimit = False
process.g4SimHits.Physics.ElectronRangeTest = False
process.g4SimHits.Physics.PositronStepLimit = False
process.g4SimHits.Physics.ProtonRegionLimit = False
process.g4SimHits.Physics.PionRegionLimit   = False

# --- SteppingAction: disable killing as much as possible

process.g4SimHits.SteppingAction.MaxNumberOfSteps = 100000000

process.g4SimHits.SteppingAction.MaxTrackTime = 1e9
process.g4SimHits.SteppingAction.MaxTrackTimeForward = 1e9
process.g4SimHits.SteppingAction.DeadRegions = []

process.g4SimHits.SteppingAction.EkinNames = []
process.g4SimHits.SteppingAction.EkinThresholds = []
process.g4SimHits.SteppingAction.EkinParticles = []

process.g4SimHits.SteppingAction.CriticalEnergyForVacuum = 0.0
process.g4SimHits.SteppingAction.CriticalDensity = 0.0

process.g4SimHits.SteppingAction.MaxZCentralCMS = 1e9
"""

# create output dir
output_dir = "configs"
os.makedirs(output_dir, exist_ok=True)

# save config file names
file_list = []

# generate config file
for Ekin in kinetic_energies:
    total_energy = Ekin + mass
    filename = f"{output_dir}/TB2006_{pname}_{Ekin}GeV_cfg.py"
    content = template.format(pname=pname, pkinetic=Ekin, total_energy=total_energy, pdgID=pdgID)
    with open(filename, "w") as f:
        f.write(content)
    file_list.append(os.path.basename(filename))
    print(f"Created: {filename} with total energy = {total_energy:.7f} GeV")

list_filename = f"{output_dir}/config_list.txt"
with open(list_filename, "w") as f:
    for fname in file_list:
        f.write(fname + "\n")
