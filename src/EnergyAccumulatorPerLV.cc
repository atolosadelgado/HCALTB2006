#include "EnergyAccumulatorPerLV.hh"
#include "G4Exception.hh"

void EnergyAccumulatorPerLV::Initialize(std::vector<std::string> & sensitive_lv)
{
    if( 0 == map.size() ) FillMap(sensitive_lv);
    else                  Reset();
}


#include "G4LogicalVolumeStore.hh"
void EnergyAccumulatorPerLV::FillMap(std::vector<std::string>& sensitive_lv)
{
    for(const auto & lvname : sensitive_lv)
    {
        G4LogicalVolumeStore * lvstore = G4LogicalVolumeStore::GetInstance();
        int counter = 0;
        for(auto & lv : *lvstore)
        {
            if( lvname == lv->GetName() ){
                 map[lv] = {};
                 counter++;
            }
        }
        if(0 == counter)
        {
            std::string message = "LV <" + lvname + "> not found";
            G4Exception("EnergyAccumulatorPerLV::FillMap()", "EnergyAccumulatorPerLV1", JustWarning, message.c_str() );
            continue;
        }
    }
}

#include <iostream>
void EnergyAccumulatorPerLV::Reset()
{
    if(0<verbosity) std::cout << __PRETTY_FUNCTION__ << std::endl;
    for(auto & lvinfo : map){
        if(0<verbosity)  std::cout << "layer " << lvinfo.first << "\tnumber of hits " << lvinfo.second.size() << std::endl;
        lvinfo.second.clear();
    }
}
