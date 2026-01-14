
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
  worldPV = Parser.GetWorldVolume();

  G4Material* scint = G4Material::GetMaterial("Scintillator");
  scint->GetIonisation()->SetBirksConstant(0.006*CLHEP::mm/CLHEP::MeV);
  G4LossTableManager::Instance()->EmSaturation()->InitialiseG4Saturation();
// G4LossTableManager::Instance()->EmSaturation()->DumpBirksCoefficients();

  if(ECALAsAir)
    this->MakeECALAsAir();

  return worldPV;
}


YourDetectorConstructor::~YourDetectorConstructor(){

}

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4GeometryManager.hh"

void YourDetectorConstructor::MakeECALAsAir()
{

  G4VPhysicalVolume* ecalPV = nullptr;

  // World -> TBHCal -> Calo (+HcalTestBeamLine, walls, etc) ->
  G4LogicalVolume* motherLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Calo");

  if(nullptr == motherLV) throw std::runtime_error("YourDetectorConstructor::SetupECALAsAir cannot find Calo LV");

  for (int i = 0; i < motherLV->GetNoDaughters(); i++) {
      G4VPhysicalVolume* daughter = motherLV->GetDaughter(i);
      std::cout << "World daughter : " << daughter->GetName() << std::endl;
      if (daughter->GetName() == "ECAL") { // nombre del physvol del ECAL
          ecalPV = daughter;
          break;
      }
  }

  if(nullptr == ecalPV) throw std::runtime_error("YourDetectorConstructor::SetupECALAsAir cannot find ECAL physical volume");
  else       G4cout << "Found ECAL physvol at position: " << ecalPV->GetTranslation() << G4endl;
  G4Material * air_mat = G4Material::GetMaterial("Air");
  if(nullptr == air_mat) throw std::runtime_error("YourDetectorConstructor::SetupECALAsAir cannot find Air material");

  ReplaceMaterialInTree(ecalPV, air_mat);

}

#include <queue>
#include <set>

void YourDetectorConstructor::ReplaceMaterialInTree(G4VPhysicalVolume* rootPV,
                            G4Material* newMat)
{
  std::queue<G4VPhysicalVolume*> queue;
  std::set<G4LogicalVolume*> visited;

  queue.push(rootPV);

  while (!queue.empty())
  {
    auto pv = queue.front();
    queue.pop();

    auto lv = pv->GetLogicalVolume();

    if (visited.insert(lv).second)
    {
      lv->SetMaterial(newMat);
    }

    for (int i = 0; i < lv->GetNoDaughters(); ++i)
    {
      queue.push(lv->GetDaughter(i));
    }
  }
}
