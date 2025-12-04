#ifndef YOURDETECTORCONSTRUCTOR
#define YOURDETECTORCONSTRUCTOR


#include "G4VUserDetectorConstruction.hh"

/// Detector construction allowing to use the geometry read from the GDML file
class YourDetectorConstructor : public G4VUserDetectorConstruction
{
  public:

  YourDetectorConstructor(std::string fname);
  ~YourDetectorConstructor() override;
  virtual G4VPhysicalVolume* Construct() override;

  private:
      std::string gdml_filename;
};

#endif
