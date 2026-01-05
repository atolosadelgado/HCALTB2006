#ifndef HISTOENERGYRESPONSE
#define HISTOENERGYRESPONSE

#include "YourHistogram.hh"

#include <memory>
#include <string>
#include "TDirectory.h"

class HistoEnergyResponse
{
public:
    HistoEnergyResponse(std::string name, int nbins, double xmin, double xmax);
    std::unique_ptr<YourHistogram> ecal;
    std::unique_ptr<YourHistogram> hcal;
    std::string fname;
    void fillEcal(double e){ ecal->fill(e); }
    void fillHcal(double e){ hcal->fill(e); };
    void write(TDirectory * d);
};

#endif
