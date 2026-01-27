#ifndef YOURDETECTORCONSTRUCTOR
#define YOURDETECTORCONSTRUCTOR


#include "G4VUserDetectorConstruction.hh"

class G4Material;
class G4VPhysicalVolume;
class G4Colour;

/// Detector construction allowing to use the geometry read from the GDML file
class YourDetectorConstructor : public G4VUserDetectorConstruction
{
  public:

  YourDetectorConstructor(std::string fname);
  ~YourDetectorConstructor() override;
  virtual G4VPhysicalVolume* Construct() override;
  virtual void ConstructSDandField() override;



  void ConfigureECALAsAir(){ECALAsAir=true;}
  void ConfigureVisSensitiveOnly(){visSensitiveOnly=true;}



  private:
      std::string gdml_filename;
      G4VPhysicalVolume * worldPV;
      G4VPhysicalVolume* ecalPV;
      G4VPhysicalVolume* hcalPV;
      std::string ecal_sensmat_name = {"E_PbWO4"};
      std::string hcal_sensmat_name = {"Scintillator"};
      bool ECALAsAir = false;;
      bool visSensitiveOnly = false;;
      void MakeECALAsAir();
      void ReplaceMaterialInTree(G4VPhysicalVolume* rootPV, G4Material* newMat);
      void HighlightMaterial(const G4String& targetMaterialName, bool makeOtherInvisible, G4Colour & color);
      void ConstructSDecal();
      void FindEcalPV();
      void FindHcalPV();
      G4VPhysicalVolume * FindPV(std::string pvname);
      void AssignLVtoSD(G4VSensitiveDetector * sd, G4VPhysicalVolume * rootPV, std::string sensmat_name);
};

#endif
