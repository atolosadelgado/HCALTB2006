#ifndef YOURDETECTORCONSTRUCTOR
#define YOURDETECTORCONSTRUCTOR

#include "G4VUserDetectorConstruction.hh"

#include "YourLayerInfo.hh"

class G4Material;
class G4VPhysicalVolume;
class G4Colour;

/// Detector construction allowing to use the geometry read from the GDML file
class YourDetectorConstructor : public G4VUserDetectorConstruction
{
  public:

  YourDetectorConstructor(std::string fname);
  ~YourDetectorConstructor() override = default;
  virtual G4VPhysicalVolume* Construct() override;
  virtual void ConstructSDandField() override;

  void ConfigureECALAsAir(){ECALAsAir=true;}
  void ConfigureVisSensitiveOnly(){visSensitiveOnly=true;}

  const G4String GetEcalSDname() const {return fEcalSDname; }
  const G4String GetHcalSDname() const {return fHcalSDname; }

  const YourLayerInfo & GetLayerInfo() const {return fLayerInfo;}

  private:
      std::string gdml_filename;
      G4int fVerbosity = {0};
      G4String fEcalSDname = {"ecalSD"};
      G4String fHcalSDname = {"hcalSD"};
      G4VPhysicalVolume * worldPV;
      bool ECALAsAir = false;;
      bool visSensitiveOnly = false;;
      void MakeECALAsAir();
      void ReplaceMaterialInTree(G4VPhysicalVolume* rootPV, G4Material* newMat);
      void HighlightMaterial(const G4String& targetMaterialName, bool makeOtherInvisible, G4Colour & color);
      // LV attached to ECalSD (dumped from CMSSW)
      std::vector<std::string> fECAL_sensLV = { "EBRY_01", "EBRY_01_refl",
                                                "EBRY_02", "EBRY_02_refl",
                                                "EBRY_03", "EBRY_03_refl",
                                                "EBRY_04", "EBRY_04_refl",
                                                "EBRY_05", "EBRY_05_refl",
                                                "EBRY_06", "EBRY_06_refl",
                                                "EBRY_07", "EBRY_07_refl",
                                                "EBRY_08", "EBRY_08_refl",
                                                "EBRY_09", "EBRY_09_refl",
                                                "EBRY_10", "EBRY_10_refl",
                                                "EBRY_11", "EBRY_11_refl",
                                                "EBRY_12", "EBRY_12_refl",
                                                "EBRY_13", "EBRY_13_refl",
                                                "EBRY_14", "EBRY_14_refl",
                                                "EBRY_15", "EBRY_15_refl",
                                                "EBRY_16", "EBRY_16_refl",
                                                "EBRY_17", "EBRY_17_refl" }; // end fECAL_sensitiveLV
      // LV attached to HCalSD (dumped from CMSSW)
      std::vector<std::string> fHCAL_sensLV = { "HBScintillatorLayer0In1",  "HBScintillatorLayer0In2",
                                                "HBScintillatorLayer1In1",  "HBScintillatorLayer1In2",
                                                "HBScintillatorLayer2In1",  "HBScintillatorLayer2In2",
                                                "HBScintillatorLayer3In1",  "HBScintillatorLayer3In2",
                                                "HBScintillatorLayer4In1",  "HBScintillatorLayer4In2",
                                                "HBScintillatorLayer5In1",  "HBScintillatorLayer5In2",
                                                "HBScintillatorLayer6In1",  "HBScintillatorLayer6In2",
                                                "HBScintillatorLayer7In1",  "HBScintillatorLayer7In2",
                                                "HBScintillatorLayer8In1",  "HBScintillatorLayer8In2",
                                                "HBScintillatorLayer9In1",  "HBScintillatorLayer9In2",
                                                "HBScintillatorLayer10In1", "HBScintillatorLayer10In2",
                                                "HBScintillatorLayer11In1", "HBScintillatorLayer11In2",
                                                "HBScintillatorLayer12In1", "HBScintillatorLayer12In2",
                                                "HBScintillatorLayer13In1", "HBScintillatorLayer13In2",
                                                "HBScintillatorLayer14In1", "HBScintillatorLayer14In2",
                                                "HBScintillatorLayer15In1", "HBScintillatorLayer15In2",
                                                "HBScintillatorLayer16In1", "HBScintillatorLayer16In2",
                                                "HEScintillatorPart3Layer02Phi0", "HEScintillatorPart3Layer02Phi1",
                                                "HEScintillatorPart3Layer03Phi0", "HEScintillatorPart3Layer03Phi1",
                                                "HEScintillatorPart3Layer04Phi0", "HEScintillatorPart3Layer04Phi1",
                                                "HEScintillatorPart3Layer05Phi0", "HEScintillatorPart3Layer05Phi1",
                                                "HEScintillatorPart3Layer06Phi0", "HEScintillatorPart3Layer06Phi1",
                                                "HEScintillatorPart4Layer07Phi0", "HEScintillatorPart4Layer07Phi1",
                                                "HEScintillatorPart4Layer08Phi0", "HEScintillatorPart4Layer08Phi1",
                                                "HEScintillatorPart4Layer09Phi0", "HEScintillatorPart4Layer09Phi1",
                                                "HEScintillatorPart4Layer10Phi0", "HEScintillatorPart4Layer10Phi1",
                                                "HEScintillatorPart4Layer11Phi0", "HEScintillatorPart4Layer11Phi1",
                                                "HEScintillatorPart4Layer12Phi0", "HEScintillatorPart4Layer12Phi1",
                                                "HEScintillatorPart5Layer13Phi0", "HEScintillatorPart5Layer13Phi1",
                                                "HEScintillatorPart5Layer14Phi0", "HEScintillatorPart5Layer14Phi1",
                                                "HEScintillatorPart5Layer15Phi0", "HEScintillatorPart5Layer15Phi1",
                                                "HEScintillatorPart5Layer16Phi0", "HEScintillatorPart5Layer16Phi1",
                                                "HEScintillatorPart5Layer17Phi0", "HEScintillatorPart5Layer17Phi1",
                                                "HEScintillatorPart5Layer18Phi0", "HEScintillatorPart5Layer18Phi1",
                                                "HTSensLayer0", "HTSensLayer1",
                                                "HTSensLayer2", "HTSensLayer3" }; // end sensitive_lv

    // Method G4VUserDetectorConstruction::SetSensitiveDetector cannot be used
    // because assumes uniqueness of Logical Volumes
    void AssignSDtoLV(std::vector<std::string> & lvnames, G4VSensitiveDetector *sd);

    // storage class to map LV -> layer number
    // this can be used by either SD or Event Action
    YourLayerInfo fLayerInfo;
    // this method contains the logic to fill fLayerInfo
    void FillLayerInfo(G4LogicalVolume * lv);
};


#endif
