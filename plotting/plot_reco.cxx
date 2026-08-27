#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TTree.h"

const std::map<std::string, double> pname_pmassGeV = {
    {"pi-", 0.1396},
    {"pi+", 0.1396},
    {"proton", 0.9383},
    {"anti_proton", 0.9383},
    {"kaon+", 0.4937},
    {"kaon-", 0.4937},
    {"e-", 0.000511},
};

// Calibration and smearing constants
constexpr double MeV_to_GeV = 1.e-3;

constexpr double ECAL_calibrationFactor = 1.01;
constexpr double HCAL_calibrationFactor = 106.5;

constexpr double ECAL_smearing = 0.362;  // GeV
constexpr double HCAL_smearing = 0.64;   // GeV

double ComputeMomentum(double ekin_GeV, const std::string& pname)
{
    const double mass_GeV = pname_pmassGeV.at(pname);

    return std::sqrt(ekin_GeV * (ekin_GeV + 2.0 * mass_GeV));
}

double processECAL(double e_ecal)
{
    return MeV_to_GeV * ECAL_calibrationFactor * e_ecal
           + gRandom->Gaus(0.0, ECAL_smearing);
}

double processHCAL(double e_hcal)
{
    return MeV_to_GeV * HCAL_calibrationFactor * e_hcal
           + gRandom->Gaus(0.0, HCAL_smearing);
}

bool fitAroundMaximum(
    TH1* h,
    int nBinsAround,
    double& mean,
    double& sigma)
{
    if (h == nullptr || h->GetEntries() == 0) {
        std::cerr << "Cannot fit histogram: empty or null histogram\n";
        return false;
    }

    // Find bin with maximum counts
    const int maxBin = h->GetMaximumBin();

    // Define range for fitting
    const int binLow =
        std::max(1, maxBin - nBinsAround);

    const int binHigh =
        std::min(h->GetNbinsX(), maxBin + nBinsAround);

    // Convert bin range to x coordinates
    const double xLow = h->GetBinLowEdge(binLow);
    const double xHigh = h->GetBinLowEdge(binHigh + 1);

    // Define fit function
    TF1 f("f", "gaus", xLow, xHigh);

    // Initialize parameters close to the expected result
    f.SetParameters(
        h->GetMaximum(),
        h->GetBinCenter(maxBin),
        h->GetRMS());

    // Fit only within the specified range.
    // Q: quiet, R: use specified range, +: keep function attached to histogram
    const int fitStatus = h->Fit(&f, "QR+");

    if (fitStatus != 0) {
        std::cerr << "Gaussian fit failed with status "
                  << fitStatus << '\n';

        mean = 0.0;
        sigma = 0.0;

        return false;
    }

    mean = f.GetParameter(1);
    sigma = f.GetParameter(2);

    return true;
}

void plot_reco(
    std::string pname = "pi-",
    double penergy_GeV = 49.861,
    double xmin = 20.0,
    double xmax = 80.0,
    int nevents = 20000,
    int airECAL = 0)
{
    // Use a fixed seed for reproducibility.
    // Change/remove this if independent random smearing is desired.
    gRandom->SetSeed(12345);

    const auto standalone_filename =
        Form("HCALTB2006_%s_%.6f_%devt_CMS_airECAL%d.root",
             pname.c_str(),
             penergy_GeV,
             nevents,
             airECAL);

    TFile* file = TFile::Open(standalone_filename);

    if (file == nullptr || file->IsZombie()) {
        std::cerr << "File <"
                  << standalone_filename
                  << "> could not be opened\n";
        return;
    }

    auto* tree = dynamic_cast<TTree*>(file->Get("tree"));

    if (tree == nullptr) {
        std::cerr << "Could not find TTree <tree> in file <"
                  << standalone_filename << ">\n";
        file->Close();
        return;
    }

    constexpr int nbins = 30;

    const auto branch_to_draw =
        Form("processECAL(ECAL_eresponse) + "
             "processHCAL(HCAL_eresponse)>>h(%d,%g,%g)",
             nbins,
             xmin,
             xmax);

    TCanvas* c1 =
        new TCanvas("c1", "c1", 10, 31, 525, 500);

    gStyle->SetOptStat(0);

    tree->Draw(branch_to_draw);

    auto* h = dynamic_cast<TH1*>(gDirectory->Get("h"));

    if (h == nullptr) {
        std::cerr << "Could not create reconstructed-energy histogram\n";
        file->Close();
        return;
    }

    h->GetXaxis()->SetRangeUser(xmin, xmax);
    h->GetYaxis()->SetLabelSize(0.05);
    h->SetMinimum(0.1);
    h->SetTitle("");
    h->Sumw2();

    // Fit around the maximum
    constexpr int nBinsAroundMaximum = 5;

    double hmean = 0.0;
    double hsigma = 0.0;

    const bool fitOK =
        fitAroundMaximum(
            h,
            nBinsAroundMaximum,
            hmean,
            hsigma);

    // Compute beam momentum from kinetic energy
    double pmomentum_GeV = 0.0;

    try {
        pmomentum_GeV =
            ComputeMomentum(penergy_GeV, pname);
    }
    catch (const std::out_of_range&) {
        std::cerr << "Unknown particle name: "
                  << pname << '\n';
        file->Close();
        return;
    }

    // Write results
    std::ofstream ofile(
        "centroids.txt",
        std::ios::app);

    if (!ofile) {
        std::cerr << "Could not open centroids.txt for writing\n";
    }
    else if (fitOK) {
        const double response =
            hmean / pmomentum_GeV;

        ofile
            << pname << '\t'
            << penergy_GeV << '\t'
            << hmean << '\t'
            << hsigma << '\t'
            << pmomentum_GeV << '\t'
            << response << '\t'
            << '\n';
    }

    // Legend
    TLegend* leg =
        new TLegend(
            0.6127591,
            0.7321938,
            0.9128024,
            0.94302,
            nullptr,
            "brNDC");

    leg->SetBorderSize(1);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);

    TLegendEntry* entry =
        leg->AddEntry(
            h,
            Form("%s at %.1f GeV",
                 pname.c_str(),
                 penergy_GeV),
            "lep");

    entry->SetFillStyle(101);
    entry->SetLineColor(1);
    entry->SetLineStyle(1);
    entry->SetLineWidth(1);
    entry->SetMarkerColor(1);
    entry->SetMarkerStyle(1);
    entry->SetMarkerSize(1);
    entry->SetTextFont(42);

    leg->Draw();

    c1->cd();

    const auto canvasFileName =
        Form("HCALTB2006_%s_%.6f_%devt_CMS_airECAL%d_reco.png",
             pname.c_str(),
             penergy_GeV,
             nevents,
             airECAL);

    c1->SaveAs(canvasFileName);

    file->Close();
}
