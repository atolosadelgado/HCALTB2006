#ifndef EVENTACCUMULATOR_HH
#define EVENTACCUMULATOR_HH

#include <unordered_map>
#include <vector>
#include <string>

class G4LogicalVolume;

class EventAccumulator
{
public:
    std::unordered_map<G4LogicalVolume*, double> map;
    void Initialize(std::vector<std::string> & sensitive_lv);
private:
    void FillMap(std::vector<std::string> & sensitive_lv);
    void Reset();
    int verbosity = {0};
};

#endif
