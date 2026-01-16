#ifndef YOURPRIMARYGENERATOR_HH
#define YOURPRIMARYGENERATOR_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4VPrimaryGenerator.hh>
#include <G4ParticleGun.hh>
#include <G4GeneralParticleSource.hh>
#include <CLHEP/Units/SystemOfUnits.h>

class YourPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    enum class SourceType {
        kParticleGun,
        kGPS
    };

    YourPrimaryGenerator(SourceType type = SourceType::kGPS);
    virtual ~YourPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event* event);

    G4double x0_mm{-999.};
    G4double y0_mm{-999.};
    G4double E0_MeV{-999.};

    std::string primary_particle_name;

private:
    G4VPrimaryGenerator* fPrimaryGen = nullptr;
    SourceType fType;

    int counter = 0;

    // Extract direction from beamline PV
    void ShowBeamLineDirection();
};

#endif
