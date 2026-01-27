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
      bool ECALAsAir = false;;
      bool visSensitiveOnly = false;;
      void MakeECALAsAir();
      void ReplaceMaterialInTree(G4VPhysicalVolume* rootPV, G4Material* newMat);
      void HighlightMaterial(const G4String& targetMaterialName, bool makeOtherInvisible, G4Colour & color);
      void ConstructSDecal();
};

#endif
