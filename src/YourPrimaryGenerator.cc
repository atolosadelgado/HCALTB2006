#include "YourPrimaryGenerator.hh"
#include "YourEventInfo.hh"

#include <iostream>

#include <G4VPrimaryGenerator.hh>
#include <G4ParticleGun.hh>
#include <G4GeneralParticleSource.hh>
#include "G4PhysicalVolumeStore.hh"

YourPrimaryGenerator::YourPrimaryGenerator(SourceType type)
: G4VUserPrimaryGeneratorAction(), fType(type)
{
    if (fType == SourceType::kParticleGun) {
        fPrimaryGen = new G4ParticleGun(1);
    } else {
        fPrimaryGen = new G4GeneralParticleSource();
    }
}

YourPrimaryGenerator::~YourPrimaryGenerator()
{
    delete fPrimaryGen;
}


void YourPrimaryGenerator::GeneratePrimaries(G4Event* event)
{
    // for debugging
    // this->ShowBeamLineDirection();

    fPrimaryGen->GeneratePrimaryVertex(event);
    YourEventInfo * info = new YourEventInfo();
    auto vertex = event->GetPrimaryVertex(0);

    info->primaryPos = vertex->GetPosition();
    info->primaryDir = vertex->GetPrimary()->GetMomentumDirection();

    event->SetUserInformation(info);

    // show information about the primary particle in first event
    if( 0<verbosity && 0 == event->GetEventID() )
    {
        G4cout << "=== Primary particle ===" << G4endl;
        G4cout << "Particle: "
                << vertex->GetPrimary()->GetParticleDefinition()->GetParticleName()
                << G4endl;

        G4cout << "Vertex position (global) [mm]: "
                << info->primaryPos.x()/CLHEP::mm << " "
                << info->primaryPos.y()/CLHEP::mm << " "
                << info->primaryPos.z()/CLHEP::mm << G4endl;

        G4cout << "Direction (unit vector): "
                << info->primaryDir.x() << " "
                << info->primaryDir.y() << " "
                << info->primaryDir.z() << G4endl;

        auto mom = vertex->GetPrimary()->GetMomentum();
        G4cout << "Momentum [GeV]: "
                << mom.x()/CLHEP::GeV << " "
                << mom.y()/CLHEP::GeV << " "
                << mom.z()/CLHEP::GeV << G4endl;

        G4cout << "Kinetic Energy [GeV]: "
                << vertex->GetPrimary()->GetKineticEnergy()/CLHEP::GeV << G4endl;

                G4cout << "Total Energy [GeV]: "
                << vertex->GetPrimary()->GetTotalEnergy()/CLHEP::GeV << G4endl;

        G4cout << "========================" << G4endl;
    }
}

void YourPrimaryGenerator::ShowBeamLineDirection()
{
    G4PhysicalVolumeStore * pvstore = G4PhysicalVolumeStore::GetInstance();
    G4VPhysicalVolume * beamline_pv = nullptr;
    for( auto & physvols : *pvstore )
    {
        if("HcalTestBeamLine" == physvols->GetName())
        {
            beamline_pv = physvols;
            break;
        }
    }

    if(not beamline_pv)
    {
        G4cout << "No HcalTestBeamLine found in physical volume store" << G4endl;
        return;
    }

    // get the rotation matrix used to place the beamline into the world
    G4RotationMatrix * m = beamline_pv->GetRotation();
    // we need the inverse transformation: from the beamline to world
    m->invert();
    // the direction of the beamline (Z axis) in world coordinates corresponds to Z column
    G4ThreeVector z_axis = m->colZ();
    G4cout << "Z axis of HcalTestBeamLine in mother coords: " << z_axis << G4endl;
    return;
}
