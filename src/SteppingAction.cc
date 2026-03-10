#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
void SteppingAction::UserSteppingAction(const G4Step * step)
{
// auto ff_matscan = [](const G4Step * step)
// {
  // if( not step->IsFirstStepInVolume() ) return;
//     const G4Track* track = step->GetTrack();
//      if (track->GetParentID() != 0) return;
//     const G4StepPoint* preStepPoint = step->GetPreStepPoint();
//
//     if (!preStepPoint) return;
//
//     G4int stepID = track->GetCurrentStepNumber();
//
//     G4ThreeVector pos = preStepPoint->GetPosition();
//     G4double stepLength = step->GetStepLength();
//
//     G4VPhysicalVolume* physVol = preStepPoint->GetPhysicalVolume();
//     if (!physVol) return;
//
//     G4LogicalVolume* logVol = physVol->GetLogicalVolume();
//     G4Material* material = logVol->GetMaterial();
//
//     G4String logVolName = logVol->GetName();
//     G4String matName = material->GetName();
//
//     // Energía media de ionización (Mean Excitation Energy)
//     G4double ionE = 0.0;
//     if (material->GetIonisation()) {
//         ionE = material->GetIonisation()->GetMeanExcitationEnergy();
//     }
//     std::ofstream("stepping_debug_transport_only.txt", std::ios::app) << stepID << "  "
//         << pos.x()/CLHEP::mm << "\t"
//         << pos.y()/CLHEP::mm << "\t"
//         << pos.z()/CLHEP::mm << "\t"
//         << stepLength/CLHEP::mm << "\t"
//         << logVolName << "\t"
//         << matName << "\t"
//         << ionE/CLHEP::eV
//         << std::endl;
// };
// ff_matscan(step);
auto ff = [](const G4Step* step){    const G4Track* track = step->GetTrack();

 // Solo part<C3><AD>cula primaria, primer step
 if (track->GetParentID() != 0) return;
 if (track->GetCurrentStepNumber() > 1) return;

 // Posici<C3><B3>n del v<C3><A9>rtice (origen)
 G4ThreeVector vtx = track->GetVertexPosition();

 // Direcci<C3><B3>n inicial (momento unitario)
 G4ThreeVector dir = track->GetMomentumDirection();

 // Momento inicial (si lo quieres)
 // G4ThreeVector mom = track->GetVertexMomentumDirection();
 G4ThreeVector mom = step->GetPreStepPoint()->GetMomentum();

 std::cout << "=== Primary particle ===" << std::endl;
 std::cout << "Particle: "
        << track->GetParticleDefinition()->GetParticleName()
        << std::endl;

 std::cout << "Vertex position (global) [mm]: "
        << vtx.x()/CLHEP::mm << " "
        << vtx.y()/CLHEP::mm << " "
        << vtx.z()/CLHEP::mm << std::endl;

 std::cout << "Direction (unit vector): "
        << dir.x() << " "
        << dir.y() << " "
        << dir.z() << std::endl;

 std::cout << "Momentum [GeV]: "
        << mom.x()/CLHEP::GeV << " "
        << mom.y()/CLHEP::GeV << " "
        << mom.z()/CLHEP::GeV << std::endl;

 std::cout << "========================" << std::endl;
  auto* ana = G4AnalysisManager::Instance();
    ana->FillNtupleDColumn(32, mom.x()/CLHEP::GeV);
    ana->FillNtupleDColumn(33, mom.y()/CLHEP::GeV);
    ana->FillNtupleDColumn(34, mom.z()/CLHEP::GeV);


};
ff(step);

}
