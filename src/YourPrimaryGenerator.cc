#include "YourPrimaryGenerator.hh"

#include <iostream>

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

    auto vertex = event->GetPrimaryVertex(0);
    x0_mm = vertex->GetPosition().x() / CLHEP::mm;
    y0_mm = vertex->GetPosition().y() / CLHEP::mm;
    E0_MeV = vertex->GetPrimary()->GetKineticEnergy() / CLHEP::MeV;

    if(0 >= E0_MeV) throw std::runtime_error("YourPrimaryGenerator::GeneratePrimaries cannot generate negative or zero energy particles");


    if( "" == primary_particle_name )
    {
        primary_particle_name = vertex->GetPrimary(0)->GetParticleDefinition()->GetParticleName();
        if(0<verbosity)
            G4cout << "\tSetting primary particle name : " << primary_particle_name << std::endl;
    }
}

#include "G4PhysicalVolumeStore.hh"
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
        G4cout << "No HcalTestBeamLine found in physical volume store" << std::endl;
        return;
    }

    // get the rotation matrix used to place the beamline into the world
    G4RotationMatrix * m = beamline_pv->GetRotation();
    // we need the inverse transformation: from the beamline to world
    m->invert();
    // the direction of the beamline (Z axis) in world coordinates corresponds to Z column
    G4ThreeVector z_axis = m->colZ();
    G4cout << "Z axis of HcalTestBeamLine in mother coords: " << z_axis << std::endl;
    return;
}
