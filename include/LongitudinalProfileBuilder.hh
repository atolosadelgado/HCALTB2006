#ifndef LONGITUDINAL_PROFILER_BUILDER_HH
#define LONGITUDINAL_PROFILER_BUILDER_HH

#include <unordered_map>
#include <vector>
#include <string>

class G4LogicalVolume;

class LongitudinalProfileBuilder
{
public:
    struct AccumulatedInfo {
        double totalEnergy = 0;
        double weightedRadius = 0;
    };
    std::unordered_map<G4LogicalVolume*, AccumulatedInfo> map;
    void AddHitInfo(G4LogicalVolume* lv,HitInfo h){map[lv].push_back(h);}
    double GetTotalEnergy();
    void Initialize(std::vector<std::string> & sensitive_lv);
private:
    void FillMap(std::vector<std::string> & sensitive_lv);
    void Reset();
    int verbosity = {0};
};

#endif
