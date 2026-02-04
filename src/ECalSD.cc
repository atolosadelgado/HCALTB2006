#include "ECalSD.h"
#include "SDutils.hh"

#include <iostream>

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4ThreeVector.hh"

// Naive implementation
// G4bool ECalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
//     double edep = getEnergyDeposit(aStep);
//     if (edep <= 0.) return false;
//     event_energy += edep;
//     if(1<verbosity)
//         std::cout << "\t[" + this->GetName() + "] " << edep/CLHEP::MeV << " MeV" << std::endl;
//
//     return true; // Step properly processed
// }

// CMSSW implementation
G4bool ECalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

    // This function throws fatal exception if position/momentum are NaNTrap
    // Function reimplemented from SensitiveDetector in CMSSW
    SDutils::NaNTrap(aStep);
    // hardcodded value as in CMSSW
    // constexpr bool ignoreReject = false;

#ifndef NDEBUG
  edm::LogVerbatim("CaloSim") << "CaloSD::" << GetName() << " ID= " << aStep->GetTrack()->GetTrackID()
                              << " prID= " << aStep->GetTrack()->GetParentID()
                              << " Eprestep= " << aStep->GetPreStepPoint()->GetKineticEnergy()
                              << " step= " << aStep->GetStepLength() << " Edep= " << aStep->GetTotalEnergyDeposit();
#endif

  // Class variable to determine whether finecalo rules should apply for this step
  // From dump, doFineCalo is false, so the line is commented
  // doFineCaloThisStep_ = (doFineCalo_ && isItFineCalo(aStep->GetPreStepPoint()->GetTouchable()));
  // From dump, doFineCaloThisStep is also false

  // apply shower library or parameterisation
  // independent on energy deposition at a step
  // From dump, isParameterized is false, so the block is commented
  // if (isParameterized) {
  //   if (getFromLibrary(aStep)) {
  //     // for parameterized showers the primary track should be killed
  //     // secondary tracks should be killed if they are in the same volume
  //     (aStep->GetTrack())->SetTrackStatus(fStopAndKill);
  //     if (0 < aStep->GetNumberOfSecondariesInCurrentStep()) {
  //       auto tv = aStep->GetSecondaryInCurrentStep();
  //       auto vol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  //       for (auto& tk : *tv) {
  //         if (tk->GetVolume() == vol) {
  //           const_cast<G4Track*>(tk)->SetTrackStatus(fStopAndKill);
  //         }
  //       }
  //     }
  //     return true;
  //   }
  // }

  // internal members to store separately the energy deposited
  // by EM or hadronic processes, removedd as not needed
  // edepositEM = edepositHAD = 0.f;

  // ignore steps without energy deposit
  if (aStep->GetTotalEnergyDeposit() <= 0.0) {
    return false;
  }

  // check unitID
  // unsigned int unitID = setDetUnitId(aStep);
  // auto const theTrack = aStep->GetTrack();
  // uint16_t depth = getDepth(aStep);

  // double time = theTrack->GetGlobalTime() / CLHEP::nanosecond;
  // int primaryID = getTrackID(theTrack);
  // From dump, doFineCaloThisStep_ is false, and only later under that condition currentID[0] is used
  // if (unitID > 0) {
  //   currentID[0].setID(unitID, time, primaryID, depth);
  // } else {
  //   if (!ignoreReject) {
  //     const G4TouchableHistory* touch = static_cast<const G4TouchableHistory*>(theTrack->GetTouchable());
  //     edm::LogVerbatim("CaloSim") << "CaloSD::ProcessHits: unitID= " << unitID
  //                                 << " currUnit=   " << currentID[0].unitID() << " Detector: " << GetName()
  //                                 << " trackID= " << theTrack->GetTrackID() << " "
  //                                 << theTrack->GetDefinition()->GetParticleName()
  //                                 << "\n Edep= " << aStep->GetTotalEnergyDeposit()
  //                                 << " PV: " << touch->GetVolume(0)->GetName()
  //                                 << " PVid= " << touch->GetReplicaNumber(0) << " MVid= " << touch->GetReplicaNumber(1);
  //   }
  //   return false;
  // }
  double energy = getEnergyDeposit(aStep);
  if (energy <= 0.0) {
    return false;
  }

  // From dump, doFineCaloThisStep_ is always false, so block is commented
  // if (doFineCaloThisStep_) {
  //   currentID[0].setID(unitID, time, findBoundaryCrossingParent(theTrack), depth);
  //   currentID[0].markAsFinecaloTrackID();
  // }

  // We do not need to distinguish between EM or had
  // if (G4TrackToParticleID::isGammaElectronPositron(theTrack)) {
  //   edepositEM = energy;
  // } else {
  //   edepositHAD = energy;
  // }
// #ifdef EDM_ML_DEBUG
//   const G4TouchableHistory* touch = static_cast<const G4TouchableHistory*>(theTrack->GetTouchable());
//   edm::LogVerbatim("CaloSim") << "CaloSD::" << GetName() << " PV:" << touch->GetVolume(0)->GetName()
//                               << " PVid=" << touch->GetReplicaNumber(0) << " MVid=" << touch->GetReplicaNumber(1)
//                               << " Unit:" << std::hex << unitID << std::dec << " Edep=" << edepositEM << " "
//                               << edepositHAD << " ID=" << theTrack->GetTrackID() << " pID=" << theTrack->GetParentID()
//                               << " E=" << theTrack->GetKineticEnergy() << " S=" << aStep->GetStepLength() << "\n "
//                               << theTrack->GetDefinition()->GetParticleName() << " primaryID= " << primaryID
//                               << " currentID= (" << currentID[0] << ") previousID= (" << previousID[0] << ")";
// #endif
//   if (!hitExists(aStep, 0)) {
//     // currentHit[0] = createNewHit(aStep, theTrack, 0);
//   } else {
// #ifdef EDM_ML_DEBUG
//     edm::LogVerbatim("DoFineCalo") << "Not creating new hit, only updating " << shortreprID(currentHit[0]);
// #endif
  // }
  // The function in CMSSW is empty...
  // processSecondHit(aStep, theTrack);

  event_energy+=energy;
  return true;
}


// double ECalSD::getEnergyDeposit(G4Step* aStep) {
//     return aStep->GetTotalEnergyDeposit();
// }

void ECalSD::Initialize(G4HCofThisEvent*) {
    event_energy = 0.0;
}

void ECalSD::EndOfEvent(G4HCofThisEvent*) {
    if(0<verbosity)
        std::cout << "Total energy [" + this->GetName() + "] " << event_energy/CLHEP::MeV << " MeV" << std::endl;
}



double ECalSD::getEnergyDeposit(const G4Step* aStep) {
  const G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  const G4Track* theTrack = aStep->GetTrack();
  double edep = aStep->GetTotalEnergyDeposit();

  // take into account light collection curve for crystals
  double weight = 1.;
  // from dump, suppressHeavy is false
  // if (suppressHeavy) {
  //   TrackInformation* trkInfo = (TrackInformation*)(theTrack->GetUserInformation());
  //   if (trkInfo) {
  //     int pdg = theTrack->GetDefinition()->GetPDGEncoding();
  //     if (!(trkInfo->isPrimary())) {  // Only secondary particles
  //       double ke = theTrack->GetKineticEnergy();
  //       if (((pdg / 1000000000 == 1 && ((pdg / 10000) % 100) > 0 && ((pdg / 10) % 100) > 0)) && (ke < kmaxIon))
  //         weight = 0;
  //       if ((pdg == 2212) && (ke < kmaxProton))
  //         weight = 0;
  //       if ((pdg == 2112) && (ke < kmaxNeutron))
  //         weight = 0;
  //     }
  //   }
  // }
  const G4LogicalVolume* lv = preStepPoint->GetTouchable()->GetVolume(0)->GetLogicalVolume();
  double wt1 = 1.0;
  // from dump, condition is true: useWeight is true and container noWeight is empty
  //if(useWeight && !any(noWeight, lv))
  // if (true) {
      // FIXME: add later correction of curve_LY (it is <1% level, disregarded in first iteration)
    // weight *= curve_LY(lv, aStep);
    // useBirk is true
    // if (cms_config.useBirk) {
        // useBirkL3 is true
      // if (cms_config.useBirkL3)
        weight *= getBirkL3(aStep);
      // else
        // weight *= getAttenuation(aStep, cms_config.birk1, cms_config.birk2, cms_config.birk3);
    // }
    // from dump, always 1, no tables are loaded
    // wt1 = getResponseWt(theTrack);
  // }
  edep *= weight * wt1;
  // this does not apply in stand alone
  // Russian Roulette
  // double wt2 = theTrack->GetWeight();
  // if (wt2 > 0.0) {
  //   edep *= wt2;
  // }
  return edep;
}

// No idea what is this caching
int ECalSD::getTrackID(const G4Track* aTrack) {
  int primaryID(0);
  // if (storeTrack && depth > 0) {
  //   forceSave = true;
    primaryID = aTrack->GetTrackID();
  // } else {
  //   primaryID = CaloSD::getTrackID(aTrack);
  // }
  return primaryID;
}

// used in process hits, reimplemented to calculate only crystalDepth, see below next method
//
// uint16_t ECalSD::getDepth(const G4Step* aStep) {
//   // this method should be called first at a step
//   const G4StepPoint* hitPoint = aStep->GetPreStepPoint();
//   currentLocalPoint = setToLocal(hitPoint->GetPosition(), hitPoint->GetTouchable());
//   const G4LogicalVolume* lv = hitPoint->GetTouchable()->GetVolume(0)->GetLogicalVolume();
//
//   auto ite = xtalLMap.find(lv);
//   crystalLength = (ite == xtalLMap.end()) ? 230._mm : std::abs(ite->second);
//   crystalDepth = (ite == xtalLMap.end()) ? 0.0 : (std::abs(0.5 * (ite->second) + currentLocalPoint.z()));
//   depth = any(useDepth1, lv) ? 1 : (any(useDepth2, lv) ? 2 : 0);
//   uint16_t depth1(0), depth2(0);
//   if (storeRL) {
//     depth1 = (ite == xtalLMap.end()) ? 0 : (((ite->second) >= 0) ? 0 : PCaloHit::kEcalDepthRefz);
//     depth2 = getRadiationLength(hitPoint, lv);
//     depth |= (((depth2 & PCaloHit::kEcalDepthMask) << PCaloHit::kEcalDepthOffset) | depth1);
//   } else if (storeLayerTimeSim) {
//     depth2 = getLayerIDForTimeSim();
//     depth |= ((depth2 & PCaloHit::kEcalDepthMask) << PCaloHit::kEcalDepthOffset);
//   }
// #ifdef EDM_ML_DEBUG
//   if (isXtal(lv))
//     edm::LogVerbatim("EcalSimX") << "ECalSD::Volume " << lv->GetName() << " DetId " << std::hex << setDetUnitId(aStep)
//                                  << std::dec << " Global " << (hitPoint->GetPosition()).rho() << ":"
//                                  << (hitPoint->GetPosition()).z() << " Local Z " << currentLocalPoint.z() << " Depth "
//                                  << crystalDepth;
//   edm::LogVerbatim("EcalSim") << "ECalSD::Depth " << std::hex << depth1 << ":" << depth2 << ":" << depth << std::dec
//                               << " L " << (ite == xtalLMap.end()) << ":" << ite->second << " local "
//                               << currentLocalPoint << " Crystal length " << crystalLength << ":" << crystalDepth;
// #endif
//   return depth;
// }
double ECalSD::computeCrystalDepth(const G4Step* aStep)
{
  if (!aStep) return 0.0;

  // global position of the step
  const G4ThreeVector& globalPos =
      aStep->GetPreStepPoint()->GetPosition();

  // transformation to local coordinates of the current volume
  const G4TouchableHistory* touchable =
      static_cast<const G4TouchableHistory*>(
          aStep->GetPreStepPoint()->GetTouchable());

  G4ThreeVector localPos =
      touchable->GetHistory()->GetTopTransform().TransformPoint(globalPos);

  // z local (origin in the center of the crystal)
  double zLocal = localPos.z();

  // depth from face APD
  double crystalDepth = std::abs(0.5 * this->cms_config.crystalLength + zLocal);

  return crystalDepth;
}

double ECalSD::curve_LY(const G4LogicalVolume* lv, const G4Step * step) {
  double weight = 1.;
  // from dump, ageingWithSlopeLY is false
  // if (ageingWithSlopeLY) {
  //   //position along the crystal in mm from 0 to 230 (in EB)
  //   if (crystalDepth >= -0.1 || crystalDepth <= crystalLength + 0.1)
  //     weight = ageing.calcLightCollectionEfficiencyWeighted(currentID[0].unitID(), crystalDepth / crystalLength);
  // } else
  // {
   double crystalDepth = this->computeCrystalDepth(step);
    double dapd = cms_config.crystalLength - crystalDepth;
    // by construction this condition is always true, but left as in CMSSW...
    if (dapd >= -0.1 || dapd <= cms_config.crystalLength + 0.1) {
      if (dapd <= 100.)
        weight = 1.0 + cms_config.slopeLY - dapd * 0.01 * cms_config.slopeLY;
    } else {
        G4cerr << "EcalSim\t" << "ECalSD: light coll curve : wrong distance "
                                 << "to APD " << dapd << " crlength = " << cms_config.crystalLength << ":" << crystalDepth
                                 << " crystal name = " << lv->GetName() << " "
                                 << lv->GetName()
                                 << " z of localPoint = " << step->GetPreStepPoint()->GetPosition().z() << " take weight = " << weight;
    }
  // }

  return weight;
}

// from CMSSW. By reading the source code, and values in cms cms_config
// it seems it will always return birkCut because birk1 is 1e18 (!!)
// it seems to implement a logaritmic correction for Birk
double ECalSD::getBirkL3(const G4Step* aStep) {
  double weight = 1.;
  const G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  double charge = preStepPoint->GetCharge();

  if (charge != 0. && aStep->GetStepLength() > 0.) {
    const G4Material* mat = preStepPoint->GetMaterial();
    double density = mat->GetDensity();
    double dedx = aStep->GetTotalEnergyDeposit() / aStep->GetStepLength();
    double rkb = cms_config.birk1 / density;
    if (dedx > 0) {
      weight = 1. - cms_config.birkSlope * log(rkb * dedx);
      if (weight < cms_config.birkCut)
        weight = cms_config.birkCut;
      else if (weight > 1.)
        weight = 1.;
    }

  }
  // std::cout << "\n New ECalSD correction: " << weight << std::endl;
  return weight;
}
