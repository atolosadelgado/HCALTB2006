#ifndef YOURPRIMARYGENERATOR_HH
#define YOURPRIMARYGENERATOR_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include "G4ThreeVector.hh"

class G4VPrimaryGenerator;

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

    G4ThreeVector position0;
    G4ThreeVector direction0;

private:
    int verbosity = 1;
    G4VPrimaryGenerator* fPrimaryGen = nullptr;
    SourceType fType;

    // Extract direction from beamline PV
    void ShowBeamLineDirection();
};

#endif
