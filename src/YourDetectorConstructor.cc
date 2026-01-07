
#include "YourDetectorConstructor.hh"
#include "G4Exception.hh"
#include "G4LossTableManager.hh"

YourDetectorConstructor::YourDetectorConstructor(std::string fname) :  G4VUserDetectorConstruction() {
    if (fname.empty())
        G4Exception("YourDetectorConstructor", "InvalidGDML",
                    FatalException, "Filename cannot be empty");

    gdml_filename = fname;
}


#include "G4GDMLParser.hh"

G4VPhysicalVolume * YourDetectorConstructor::Construct(){
  G4GDMLParser Parser;
  Parser.Read(gdml_filename, false);
  auto worldPV = Parser.GetWorldVolume();

  G4Material* scint = G4Material::GetMaterial("Scintillator");
  scint->GetIonisation()->SetBirksConstant(0.006*CLHEP::mm/CLHEP::MeV);
  G4LossTableManager::Instance()->EmSaturation()->InitialiseG4Saturation();
// G4LossTableManager::Instance()->EmSaturation()->DumpBirksCoefficients();

  return worldPV;
}


YourDetectorConstructor::~YourDetectorConstructor(){

}
