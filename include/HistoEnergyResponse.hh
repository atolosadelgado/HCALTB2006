#ifndef HISTOENERGYRESPONSE
#define HISTOENERGYRESPONSE

#include "YourHistogram.hh"

#include <memory>
#include <string>
#include "TDirectory.h"
#include "TH2D.h"

class HistoEnergyResponse
{
public:
    HistoEnergyResponse(std::string name, int nbins, double xmin, double xmax);
    std::unique_ptr<YourHistogram> ecal;
    std::unique_ptr<YourHistogram> hcal;
    std::unique_ptr<TH2D> hEHcalos;
    std::string fname;
    void fillEcal(double e){ ecal->fill(e); }
    void fillHcal(double e){ hcal->fill(e); };
    void write(TDirectory * d);
};

#endif
