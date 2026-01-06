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
    fPrimaryGen->GeneratePrimaryVertex(event);

    auto vertex = event->GetPrimaryVertex(0);
    x0_mm = vertex->GetPosition().x() / CLHEP::mm;
    y0_mm = vertex->GetPosition().y() / CLHEP::mm;
    E0_MeV = vertex->GetPrimary()->GetKineticEnergy() / CLHEP::MeV;

    if( "" == primary_particle_name )
    {
        primary_particle_name = vertex->GetPrimary(0)->GetParticleDefinition()->GetParticleName();
    }
}

