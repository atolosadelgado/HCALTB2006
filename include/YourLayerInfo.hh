#ifndef YOURLAYERINFO_HH
#define YOURLAYERINFO_HH

#include "G4LogicalVolume.hh"
#include "G4Exception.hh"

#include <unordered_map>
#include <limits>

struct YourLayerInfo
{
    using layerNumber_t = int;

    layerNumber_t GetMaxLayerNumber() const {return maxLayer;}

    void AddLV(G4LogicalVolume* lv, layerNumber_t nlayer) {
        auto [it, inserted] = fLayerInfo.emplace(lv, nlayer);
        if (inserted)
            maxLayer = std::max(maxLayer, nlayer);
        else{
            G4ExceptionDescription msg;
            msg << "Logical Volume <" << lv->GetName() << ">already inserted in Layer info map" << G4endl;
            G4Exception("YourLayerInfo::AddLV", "Code001", JustWarning, msg);
        }

    }

    /// This method returns the associated layer number to lv
    /// if lv was not added, return NO_LAYER
    layerNumber_t GetLayer(G4LogicalVolume * lv) const {
        const auto it = fLayerInfo.find(lv);
        return (it != fLayerInfo.end()) ? it->second : NO_LAYER;
    }

    void Print(){
        G4cout << "YourLayerInfo::Print\n";
        for(auto & [lv, n] : fLayerInfo){
            G4cout << "\t" << lv->GetName() << "\t->\t" << n << std::endl;
        }
    }

    static constexpr layerNumber_t NO_LAYER = std::numeric_limits<layerNumber_t>::min();


private:
    std::unordered_map<G4LogicalVolume*,layerNumber_t> fLayerInfo;
    layerNumber_t maxLayer = {NO_LAYER};

};

#endif
