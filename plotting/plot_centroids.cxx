#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TLine.h"
#include "TStyle.h"
#include "TAxis.h"


void plot_centroids(
    const std::string& filename = "centroids.txt",
    const std::string& dataDir = "../HCALTB2006/data/experiment"
)
{
    // ---------------------------------------------------------------------
    // Particle types
    // ---------------------------------------------------------------------

    const std::vector<std::string> particles = {
        "pi+",
        "pi-",
        "proton",
        "anti_proton",
        "kaon+",
        "kaon-"
    };


    // ---------------------------------------------------------------------
    // Experimental file names
    // ---------------------------------------------------------------------

    const std::map<std::string, std::string> experimentFiles = {
        {"pi+",         "pip.txt"},
        {"pi-",         "pim.txt"},
        {"proton",      "prop.txt"},
        {"anti_proton", "prom.txt"},
        {"kaon+",       "kp.txt"},
        {"kaon-",       "km.txt"}
    };


    // ---------------------------------------------------------------------
    // Read simulation results
    //
    // Columns:
    // particle
    // kinetic energy
    // centroid
    // sigma
    // momentum
    // response
    // ---------------------------------------------------------------------

    std::map<std::string, std::vector<double>> momentum;
    std::map<std::string, std::vector<double>> response;
    std::map<std::string, std::vector<double>> sigma;
    std::map<std::string, std::vector<double>> eleak;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file <"
                  << filename << ">\n";
        return;
    }

    std::string pname;
    double kineticEnergy;
    double centroid;
    double sigmaValue;
    double p;
    double responseValue;
    double eleakValue;

    while (file >> pname
                >> kineticEnergy
                >> centroid
                >> sigmaValue
                >> p
                >> responseValue
                >> eleakValue) {

        momentum[pname].push_back(p);
        response[pname].push_back(responseValue);
        sigma[pname].push_back(sigmaValue);
        eleak[pname].push_back(eleakValue);
    }

    file.close();


    // ---------------------------------------------------------------------
    // ROOT settings
    // ---------------------------------------------------------------------

    gROOT->SetBatch(kTRUE);
    gStyle->SetOptStat(0);


    // ---------------------------------------------------------------------
    // Loop over particles
    // ---------------------------------------------------------------------

    for (const auto& particle : particles) {

        const int n = momentum[particle].size();

        if (n == 0) {
            std::cerr << "No simulation data found for "
                      << particle << '\n';
            continue;
        }

        std::cout << "Plotting "
                  << particle
                  << " (" << n << " simulation points)\n";


        // ================================================================
        // Read experimental data
        // ================================================================

        std::vector<double> expMomentum;
        std::vector<double> expResponse;
        std::vector<double> expError;

        const std::string expFilename =
            dataDir + "/" + experimentFiles.at(particle);

        std::ifstream expFile(expFilename);

        if (!expFile.is_open()) {

            std::cerr << "Could not open experimental file <"
                      << expFilename << ">\n";

        } else {

            double expP;
            double expResp;
            double expErr;

            while (expFile >> expP
                           >> expResp
                           >> expErr) {

                expMomentum.push_back(expP);
                expResponse.push_back(expResp);
                expError.push_back(expErr);
            }

            expFile.close();
        }


        // ================================================================
        // Response plot
        // ================================================================

        TGraph* gResponse =
            new TGraph(
                n,
                momentum[particle].data(),
                response[particle].data()
            );

        gResponse->SetMarkerStyle(20);
        gResponse->SetMarkerSize(1.1);
        gResponse->SetLineWidth(2);

        gResponse->SetTitle(
            Form("%s response", particle.c_str())
        );

        gResponse->GetXaxis()->SetTitle(
            "Momentum [GeV]"
        );

        gResponse->GetYaxis()->SetTitle(
            "E_{vis} / p"
        );


        double maxMomentum =
            *std::max_element(
                momentum[particle].begin(),
                momentum[particle].end()
            );

        if (!expMomentum.empty()) {

            maxMomentum = std::max(
                maxMomentum,
                *std::max_element(
                    expMomentum.begin(),
                    expMomentum.end()
                )
            );
        }

        gResponse->GetXaxis()->SetLimits(
            1.0,
            1.05 * maxMomentum
        );


        TCanvas* cResponse =
            new TCanvas(
                Form("cResponse_%s", particle.c_str()),
                Form("%s response", particle.c_str()),
                800,
                600
            );

        cResponse->SetGrid();
        cResponse->SetLogx();

        gResponse->Draw("APL");


        // ---------------------------------------------------------------
        // Experimental data
        // ---------------------------------------------------------------

        TGraphErrors* gExp = nullptr;

        if (!expMomentum.empty()) {

            const int nExp = expMomentum.size();

            gExp =
                new TGraphErrors(
                    nExp,
                    expMomentum.data(),
                    expResponse.data(),
                    nullptr,
                    expError.data()
                );

            gExp->SetMarkerStyle(21);
            gExp->SetMarkerSize(1.2);
            gExp->SetLineWidth(1);

            gExp->Draw("P SAME");
        }


        // ---------------------------------------------------------------
        // Evis/p = 1 reference line
        // ---------------------------------------------------------------

        double xMin = cResponse->GetUxmin();
        double xMax = cResponse->GetUxmax();

        TLine* responseLine =
            new TLine(
                xMin,
                1.0,
                xMax,
                1.0
            );

        responseLine->SetLineStyle(2);
        responseLine->Draw();


        // ---------------------------------------------------------------
        // Legend
        // ---------------------------------------------------------------

        TLegend* legResponse =
            new TLegend(
						  0.18,
						  0.70,
						  0.45,
						  0.88
            );

        legResponse->SetBorderSize(0);
        legResponse->SetFillStyle(0);

        legResponse->AddEntry(
            gResponse,
            "Geant4",
            "lp"
        );

        if (gExp != nullptr) {

            legResponse->AddEntry(
                gExp,
                "Experiment",
                "p"
            );
        }

        legResponse->Draw();


        // ---------------------------------------------------------------
        // Save response plot
        // ---------------------------------------------------------------

        cResponse->SaveAs(
            Form("response_%s.png", particle.c_str())
        );
        gResponse->SaveAs(
            Form("response_%s.root", particle.c_str())
        );


        // ================================================================
        // Sigma plot
        // ================================================================

        TGraph* gSigma =
            new TGraph(
                n,
                momentum[particle].data(),
                sigma[particle].data()
            );

        gSigma->SetMarkerStyle(20);
        gSigma->SetMarkerSize(1.1);
        gSigma->SetLineWidth(2);

        gSigma->SetTitle(
            Form("%s energy RMS", particle.c_str())
        );

        gSigma->GetXaxis()->SetTitle(
            "Momentum [GeV]"
        );

        gSigma->GetYaxis()->SetTitle(
            "#sigma [GeV]"
        );

        gSigma->GetXaxis()->SetLimits(
            0.0,
            1.05 * maxMomentum
        );


        TCanvas* cSigma =
            new TCanvas(
                Form("cSigma_%s", particle.c_str()),
                Form("%s sigma", particle.c_str()),
                800,
                600
            );

        cSigma->SetGrid();

        gSigma->Draw("APL");


        // ---------------------------------------------------------------
        // Save sigma plot
        // ---------------------------------------------------------------

        cSigma->SaveAs(
            Form("sigma_%s.png", particle.c_str())
        );

        gSigma->SaveAs(
            Form("sigma_%s.root", particle.c_str())
        );

        // ================================================================
        // Eleak plot
        // ================================================================

        TGraph* gEleak =
            new TGraph(
                n,
                momentum[particle].data(),
                eleak[particle].data()
            );

        gEleak->SetMarkerStyle(20);
        gEleak->SetMarkerSize(1.1);
        gEleak->SetLineWidth(2);

        gEleak->SetTitle(
            Form("%s Mean Energy leakage", particle.c_str())
        );

        gEleak->GetXaxis()->SetTitle(
            "Momentum [GeV]"
        );

        gEleak->GetYaxis()->SetTitle(
            "Mean Energy leakage [GeV]"
        );

        gEleak->GetXaxis()->SetLimits(
            0.0,
            1.05 * maxMomentum
        );


        TCanvas* cEleak =
            new TCanvas(
                Form("cEleak_%s", particle.c_str()),
                Form("%s Eleak", particle.c_str()),
                800,
                600
            );

        cEleak->SetGrid();

        gEleak->Draw("APL");


        // ---------------------------------------------------------------
        // Save sigma plot
        // ---------------------------------------------------------------

        cEleak->SaveAs(
            Form("eleak_%s.png", particle.c_str())
        );

        gEleak->SaveAs(
            Form("eleak_%s.root", particle.c_str())
        );

        // ---------------------------------------------------------------
        // Cleanup
        // ---------------------------------------------------------------

        delete cResponse;
        delete cSigma;
        delete gResponse;
        delete gSigma;
        delete gExp;
        delete responseLine;
        delete legResponse;
    }
}
