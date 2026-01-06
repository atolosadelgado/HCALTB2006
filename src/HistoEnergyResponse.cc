#include <HistoEnergyResponse.hh>

HistoEnergyResponse::HistoEnergyResponse(std::string name, int nbins, double xmin, double xmax)
{
    ecal = std::make_unique<YourHistogram>(nbins, xmin, xmax);
    hcal = std::make_unique<YourHistogram>(nbins, xmin, xmax);
    fname = name;
    std::string h2title = "visible energy in ECAL vs HCAL";
    std::string h2name = fname + "_ECAL_vs_HCAL";
    hEHcalos = std::make_unique<TH2D>( h2name.c_str() , h2title.c_str(), nbins, xmin, xmax, nbins, xmin, xmax  );
    hEHcalos->SetDirectory(0);
}

void HistoEnergyResponse::write ( TDirectory* d )
{
    if( nullptr == d ) return;

    d->cd();

    TH1D * hECAL = new TH1D(Form("%s_ECAL", fname.c_str()), "Energy response", ecal->getNBins(), ecal->getXMin(), ecal->getXMax());
    ecal->FillTH1(hECAL);
    hECAL->SetDirectory(d);
    hECAL->Write();

    TH1D * hHCAL = new TH1D(Form("%s_HCAL", fname.c_str()), "Energy response", hcal->getNBins(), hcal->getXMin(), hcal->getXMax());
    hcal->FillTH1(hHCAL);
    hHCAL->SetDirectory(d);
    hHCAL->Write();

    hEHcalos->Write();
}
