#ifndef YOURDETECTORCONSTRUCTOR
#define YOURDETECTORCONSTRUCTOR


#include "G4VUserDetectorConstruction.hh"

class G4Material;
class G4VPhysicalVolume;

/// Detector construction allowing to use the geometry read from the GDML file
class YourDetectorConstructor : public G4VUserDetectorConstruction
{
  public:

  YourDetectorConstructor(std::string fname);
  ~YourDetectorConstructor() override;
  virtual G4VPhysicalVolume* Construct() override;

  void ConfigureECALAsAir(){ECALAsAir=true;}

  private:
      std::string gdml_filename;
      G4VPhysicalVolume * worldPV;
      bool ECALAsAir = false;;
      void MakeECALAsAir();
      void ReplaceMaterialInTree(G4VPhysicalVolume* rootPV, G4Material* newMat);
};

#endif
