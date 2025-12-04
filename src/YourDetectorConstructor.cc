
#include "YourDetectorConstructor.hh"
#include "G4Exception.hh"

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
  return worldPV;
}


YourDetectorConstructor::~YourDetectorConstructor(){

}
