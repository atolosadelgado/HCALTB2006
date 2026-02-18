#include "SteppingAction.hh"
#include "G4Step.hh"

void SteppingAction::UserSteppingAction(const G4Step * step)
{
auto ff_matscan = [](const G4Step * step)
{
  // if( not step->IsFirstStepInVolume() ) return;
    const G4Track* track = step->GetTrack();
     if (track->GetParentID() != 0) return;
    const G4StepPoint* preStepPoint = step->GetPreStepPoint();

    if (!preStepPoint) return;

    G4int stepID = track->GetCurrentStepNumber();

    G4ThreeVector pos = preStepPoint->GetPosition();
    G4double stepLength = step->GetStepLength();

    G4VPhysicalVolume* physVol = preStepPoint->GetPhysicalVolume();
    if (!physVol) return;

    G4LogicalVolume* logVol = physVol->GetLogicalVolume();
    G4Material* material = logVol->GetMaterial();

    G4String logVolName = logVol->GetName();
    G4String matName = material->GetName();

    // Energía media de ionización (Mean Excitation Energy)
    G4double ionE = 0.0;
    if (material->GetIonisation()) {
        ionE = material->GetIonisation()->GetMeanExcitationEnergy();
    }
    std::ofstream("stepping_debug_transport_only.txt", std::ios::app) << stepID << "  "
        << pos.x()/CLHEP::mm << "\t"
        << pos.y()/CLHEP::mm << "\t"
        << pos.z()/CLHEP::mm << "\t"
        << stepLength/CLHEP::mm << "\t"
        << logVolName << "\t"
        << matName << "\t"
        << ionE/CLHEP::eV
        << std::endl;
};
ff_matscan(step);

}
