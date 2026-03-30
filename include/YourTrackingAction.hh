#ifndef YOURTRACKINGACTION_HH
#define YOURTRACKINGACTION_HH

#include "YourParticleInfo.hh"
#include <map>
#include <unordered_map>

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;
class G4ParticleDefinition;

class YourTrackingAction : public G4UserTrackingAction
{
public:
    YourTrackingAction();
    virtual ~YourTrackingAction();

    virtual void PostUserTrackingAction(const G4Track * track) override;

    void SetParticleInfoMap(YourParticleInfoMap & m){fParticleInfoMap = m;}
    YourParticleInfoMap fParticleInfoMap;
    const YourParticleInfo & GetParticleInfo(const G4Track * track) const;
    std::map<int, std::pair<const G4ParticleDefinition*,double>> trackIDmap;
    std::unordered_map<std::string,int> fProcNameId = {{"muBrems", 1},
                                                        {"muPairProd", 2},
                                                        {"alphaInelastic", 3},
                                                        {"ionInelastic", 4},
                                                        {"positronNuclear", 5},
                                                        {"anti_lambdaInelastic", 6},
                                                        {"He3Inelastic", 7},
                                                        {"sigma+Inelastic", 8},
                                                        {"tInelastic", 9},
                                                        {"anti_sigma-Inelastic", 10},
                                                        {"electronNuclear", 11},
                                                        {"anti_neutronInelastic", 12},
                                                        {"hIoni", 13},
                                                        {"nCapture", 14},
                                                        {"pi+Inelastic", 15},
                                                        {"sigma-Inelastic", 16},
                                                        {"hBertiniCaptureAtRest", 17},
                                                        {"anti_sigma+Inelastic", 18},
                                                        {"muMinusCaptureAtRest", 19},
                                                        {"eIoni", 20},
                                                        {"eBrem", 21},
                                                        {"Decay", 22},
                                                        {"phot", 23},
                                                        {"compt", 24},
                                                        {"hFritiofCaptureAtRest", 25},
                                                        {"annihil", 26},
                                                        {"muIoni", 27},
                                                        {"neutronInelastic", 28},
                                                        {"hadElastic", 29},
                                                        {"protonInelastic", 30},
                                                        {"photonNuclear", 31},
                                                        {"CoulombScat", 32},
                                                        {"anti_protonInelastic", 33},
                                                        {"kaon0LInelastic", 34},
                                                        {"hPairProd", 35},
                                                        {"kaon+Inelastic", 36},
                                                        {"conv", 37},
                                                        {"dInelastic", 38},
                                                        {"lambdaInelastic", 39},
                                                        {"kaon0SInelastic", 40},
                                                        {"kaon-Inelastic", 41},
                                                        {"pi-Inelastic", 42},
                                                        {"ionIoni", 43},
                                                        {"hBrems", 44},
                                                    };
};

#endif
