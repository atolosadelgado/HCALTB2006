#include "YourDetectorConstructor.hh"
#include "YourCaloSD.hh"
#include "YourEcalResponse.hh"
#include "YourHcalResponse.hh"

#include <queue>
#include <set>

#include "G4GDMLParser.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Exception.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

YourDetectorConstructor::YourDetectorConstructor(std::string fname) :  G4VUserDetectorConstruction() {
    if (fname.empty())
        G4Exception("YourDetectorConstructor", "InvalidGDML",
                    FatalException, "Filename cannot be empty");
    gdml_filename = fname;
}

G4VPhysicalVolume * YourDetectorConstructor::Construct(){
  G4GDMLParser Parser;
  Parser.Read(gdml_filename, false);
  worldPV = Parser.GetWorldVolume();

  if(ECALAsAir)
    this->MakeECALAsAir();

  if(visSensitiveOnly)
  {
    G4Colour green(0.0, 1.0, 0.0, 0.5);
    G4Colour red(1.0, 0.0, 0.0, 1.0);
    G4Colour blue(0.0, 0.0, 1.0, 0.5);

    HighlightMaterial("E_PbWO4", true, green);
    HighlightMaterial("Scintillator", false, blue);
  }

  return worldPV;
}

void YourDetectorConstructor::ConstructSDandField()
{
  // register SD objects
  YourCaloSD * ecalSD = new YourCaloSD(fEcalSDname, std::make_unique<YourEcalResponse>() );
  G4SDManager::GetSDMpointer()->AddNewDetector(ecalSD);
  AssignSDtoLV(fECAL_sensLV, ecalSD);
  YourCaloSD * hcalSD = new YourCaloSD(fHcalSDname, std::make_unique<YourHcalResponse>() );
  G4SDManager::GetSDMpointer()->AddNewDetector(hcalSD);
  AssignSDtoLV(fHCAL_sensLV, hcalSD);
}

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

  return;

}

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

  return;
}



void YourDetectorConstructor::HighlightMaterial(const G4String& targetMaterialName, bool makeOtherInvisible, G4Colour & color) {
    auto lvStore = G4LogicalVolumeStore::GetInstance();

    for (auto lv : *lvStore) {
        G4Material* mat = lv->GetMaterial();


        if (mat->GetName() == targetMaterialName) {
            G4VisAttributes* visAtt = nullptr;
            // Material objetivo: verde sólido
            visAtt = new G4VisAttributes( color );
            visAtt->SetVisibility(true);
            visAtt->SetForceSolid(true);
            lv->SetVisAttributes(visAtt);
            std::cout << " New crystal for visualization: " << lv->GetName() << std::endl;
        } else if(makeOtherInvisible){
            // Todos los demás: invisibles
            lv->SetVisAttributes( G4VisAttributes::GetInvisible() );
        }
    } // end loop over LV store

    return;
}

void YourDetectorConstructor::AssignSDtoLV(std::vector<std::string>& lvnames, G4VSensitiveDetector* sd)
{
    auto * lvstore = G4LogicalVolumeStore::GetInstance();

    for(const auto & lvname : lvnames )
    {
      // in case there are several lv with the same name
      int lvcounter = 0;
      for(auto lv : *lvstore)
      {
        if(lv->GetName() == lvname)
        {
          lv->SetSensitiveDetector(sd);
          ++lvcounter;
          if(0<fVerbosity)
            G4cout << "\t" << sd->GetName() <<" SD assigned to LV <" << lvname << ">" << G4endl;
        }
      }
      // check if there was at least 1 volume with lvname
      if( 0 == lvcounter )
        G4cerr << "\tECAL SD volume <" << lvname << "> not found" << G4endl;

    } // end loop over lvnames

    return;
}

