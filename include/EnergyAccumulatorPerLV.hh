#ifndef ENERGYACCUMuLATORPERLV_HH
#define ENERGYACCUMuLATORPERLV_HH

#include <unordered_map>
#include <vector>
#include <string>

class G4LogicalVolume;

class EnergyAccumulatorPerLV
{
public:
    struct HitInfo
    {
        double energy;
        double radius;
    };
    std::unordered_map<G4LogicalVolume*, std::vector<HitInfo>> map;
    void AddHitInfo(G4LogicalVolume* lv,HitInfo h){map[lv].push_back(h);}
    double GetTotalEnergy();
    void Initialize(std::vector<std::string> & sensitive_lv);
private:
    void FillMap(std::vector<std::string> & sensitive_lv);
    void Reset();
    int verbosity = {0};
};

#endif
