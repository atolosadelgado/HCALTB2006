#include "YourDetectorConstructor.hh"
#include "YourCaloSD.hh"
#include "YourEcalResponse.hh"
#include "YourHcalResponse.hh"
#include "YourClusterCut.hh"

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
  // The first 4 levels of the geometry tree are shown below
  // The tree is generated with Geant4 /vis/drawTree + grep -E '^ {4,10}"'
  // Format is: PV:n / LV (SD,RO)
  // "TBHCal":1 / "TBHCal"
  //   "Calo":1 / "Calo"
  //     "HCal":1 / "HCal"
  //       "HcalCable":1 / "HcalCable"
  //       "HB":1 / "HB"
  //       "HE":1 / "HE"
  //     "ECAL":1 / "ECAL"
  //       "EBAR":1 / "EBAR"
  //   "HcalTestBeamLine":1 / "HcalTestBeamLine"
  //     "HcalTestBeamLineWChamb":1 / "HcalTestBeamLineWChamb"
  //       "HcalTestBeamLineWChambGas":1 / "HcalTestBeamLineWChambGas"
  //       "HcalTestBeamLineWChambAl1":1-8 / "HcalTestBeamLineWChambAl1"
  //       "HcalTestBeamLineWChambAl2":1-4 / "HcalTestBeamLineWChambAl2"
  //       "HcalTestBeamLineWChambWindow":1,2 / "HcalTestBeamLineWChambWindow"
  //     "HcalTestBeamLineWChamb":2,3 (repeated LV) / "HcalTestBeamLineWChamb"
  //     "HcalTestBeamLineS1":1 / "HcalTestBeamLineS1"
  //     "HcalTestBeamLineS2":2 / "HcalTestBeamLineS2"
  //     "HcalTestBeamLineS3":3 / "HcalTestBeamLineS3"
  //     "HcalTestBeamLineS4":4 / "HcalTestBeamLineS4"
  //     "HcalTestBeamLineTOFCounter":5 / "HcalTestBeamLineTOFCounter"
  //     "HcalTestBeamLineHaloCounter":11-14 / "HcalTestBeamLineHaloCounter"
  //     "HcalTestBeamLineCK3":0 / "HcalTestBeamLineCK3"
  //   "HcalTestBeamLineDown":1 / "HcalTestBeamLineDown"
  //     "HcalTestBeamLineWChamb":4,5 (repeated LV) / "HcalTestBeamLineWChamb"
  //     "HcalTestBeamLineMuonCounter":7,8 / "HcalTestBeamLineMuonCounter"
  //     "HcalTestBeamLineIronBlock":1 / "HcalTestBeamLineIronBlock"
  //   "HadronOuter":0 / "HadronOuter"
  //     "HadronOuterYoke":1 / "HadronOuterYoke"
  //     "HadronOuterTail":1 / "HadronOuterTail"
  //     "HadronOuterLayer0":1 / "HadronOuterLayer0"
  //       "HadronOuterLayer0Tray":2 / "HadronOuterLayer0Tray"
  //       "HadronOuterLayer0Tray":3-6 (repeated LV) / "HadronOuterLayer0Tray"
  //     "HadronOuterLayer1":1 / "HadronOuterLayer1"
  //       "HadronOuterLayer1Tray":2 / "HadronOuterLayer1Tray"
  //       "HadronOuterLayer1Tray":3-6 (repeated LV) / "HadronOuterLayer1Tray"
  //     "HadronOuterLayer2":1 / "HadronOuterLayer2"
  //       "HadronOuterLayer2Tray":1 / "HadronOuterLayer2Tray"
  //       "HadronOuterLayer2Tray":2-6 (repeated LV) / "HadronOuterLayer2Tray"
  //     "HadronOuterLayer3":1 / "HadronOuterLayer3"
  //       "HadronOuterLayer3Tray":1 / "HadronOuterLayer3Tray"
  //       "HadronOuterLayer3Tray":2-6 (repeated LV) / "HadronOuterLayer3Tray"
  //     "HadronOuterCoil":1 / "HadronOuterCoil"
  //   "VMWall1":1 / "VMWall1"
  //     "VMWallCounter":21-24 / "VMWallCounter"
  //   "VMWall2":1 / "VMWall2"
  //     "VMWallCounter":25-28 / "VMWallCounter"



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

  if(0<fVerbosity)
    this->ShowLVperRegion();

  return worldPV;
}

void YourDetectorConstructor::ConstructSDandField()
{
  // register SD objects
  std::unique_ptr<YourClusterCut> ecalClusterCut;
  ecalClusterCut = std::make_unique<YourClusterCut>(8.5*CLHEP::cm);
  YourCaloSD * ecalSD = new YourCaloSD(fEcalSDname,
                                       std::make_unique<YourEcalResponse>(),
                                       std::move(ecalClusterCut)
                                       );
  G4SDManager::GetSDMpointer()->AddNewDetector(ecalSD);
  AssignSDtoLV(fECAL_sensLV, ecalSD);


  std::unique_ptr<YourClusterCut> hcalClusterCut;
  hcalClusterCut = std::make_unique<YourClusterCut>(28.2*CLHEP::cm);
  YourCaloSD * hcalSD = new YourCaloSD(fHcalSDname,
                                       std::make_unique<YourHcalResponse>(),
                                       std::move(hcalClusterCut)
                                       );
  G4SDManager::GetSDMpointer()->AddNewDetector(hcalSD);
  AssignSDtoLV(fHCAL_sensLV, hcalSD);
  if(0<fVerbosity)
    fLayerInfo.Print();
}

void YourDetectorConstructor::MakeECALAsAir()
{

  G4VPhysicalVolume* ecalPV = nullptr;

  // World -> TBHCal -> Calo (+HcalTestBeamLine, walls, etc) ->
  G4LogicalVolume* motherLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Calo");

  if(nullptr == motherLV) throw std::runtime_error("YourDetectorConstructor::SetupECALAsAir cannot find Calo LV");

  for (std::size_t i = 0; i < motherLV->GetNoDaughters(); i++) {
      G4VPhysicalVolume* daughter = motherLV->GetDaughter(i);
      std::cout << "World daughter : " << daughter->GetName() << std::endl;
      if (daughter->GetName() == "ECAL") { // physvol name of ECAL
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

    for (std::size_t i = 0; i < lv->GetNoDaughters(); ++i)
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
            // make the goal material solid and visible
            visAtt = new G4VisAttributes( color );
            visAtt->SetVisibility(true);
            visAtt->SetForceSolid(true);
            lv->SetVisAttributes(visAtt);
            std::cout << " New crystal for visualization: " << lv->GetName() << std::endl;
        } else if(makeOtherInvisible){
            // make anything else invisible
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
          FillLayerInfo(lv);
          if(0<fVerbosity)
            G4cout << "\t" << sd->GetName() <<" SD assigned to LV <" << lvname << ">" << G4endl;
        }
      }
      // check if there was at least 1 volume with lvname
      if( 0 == lvcounter )
        G4cerr << "\tSD volume <" << lvname << "> not found" << G4endl;

    } // end loop over lvnames

    return;
}

void YourDetectorConstructor::FillLayerInfo(G4LogicalVolume* lv)
{
  if(nullptr == lv) return;
  std::string  lvname = lv->GetName();

  // if ECAL
  if(std::size_t found = lvname.find("EBRY_"); found!=std::string::npos){
    // ECAL crystals are named as EBRY_*
    // ECAL is homogeneous, 1 single layer
    // assigning 1 as layer number for convenience
    fLayerInfo.AddLV(lv,1);
  }
  // if HCAL barrel
  else if(std::size_t found = lvname.find("HBScintillatorLayer"); found!=std::string::npos){
    // number of HCAL layer is extracted from LV name
    size_t posLayer = lvname.find("Layer") + 5;
    size_t posIn = lvname.size() - 3; // "In1" o "In2"
    int hcal_nlayer = std::stoi(lvname.substr(posLayer, posIn - posLayer));
    // HCAL layers will start at layer 2, but in the name they start at 0
    // so we add an offset of 2
    fLayerInfo.AddLV(lv,hcal_nlayer+2);
  }
  else
    // default layer of anything else
    fLayerInfo.AddLV(lv,0);

}
#include "G4RegionStore.hh"
void YourDetectorConstructor::ShowLVperRegion() const
{
  std::map<G4Region*,std::vector<G4LogicalVolume*>> region_lv_map;

  for( auto region : * G4RegionStore::GetInstance())
    region_lv_map[region] = {};

  for(auto lv : * G4LogicalVolumeStore::GetInstance() )
    if(lv->GetRegion())
      region_lv_map[lv->GetRegion()].push_back(lv);

  G4cout << "List of logical volumes in each region:\n";
  for(auto [region, LVvector] : region_lv_map)
  {
    G4cout << "-Region: " << region->GetName() << G4endl;
    for(auto lv : LVvector)
      G4cout << "\t" << lv->GetName() << G4endl;


  }
}
