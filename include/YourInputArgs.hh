#ifndef YOURINPUTARGS
#define YOURINPUTARGS

#include <string>
#include <ostream>
#include <iomanip> // std::fixed, std::setprecision

#if HAVE_ROOT

#include "TDirectory.h"
#include "TParameter.h"
#include "TFile.h"

#endif

#include "G4Version.hh"

struct YourInputArgs {

    // ----- Required -----
    std::string geometry;       // -g
    std::string physics_list;   // -pl

    // ----- Optional -----
    std::string macro;          // -m
    std::string output;         // -o

    double particle_energy_GeV = -1.0;   // -penergy
    std::string particle_name;           // -pname

    int nevents = 0;            // -nevents
    bool vis_mode = false;      // -vis

    bool airECAL = false;

    bool visSensitiveOnly = false;

    double f_ECAL = {1.};
    double f_HCAL = {1.};

    int nthreads = 1;

#if HAVE_ROOT
    void Write(TDirectory * d) const {
        if (!d) return;

        d->cd();
        TDirectory * subdir = d->mkdir("inputArgs");
        subdir->cd();

        // ---------- Required ----------
        (new TNamed("geometry", geometry.c_str()))->Write();
        (new TNamed("physics_list", physics_list.c_str()))->Write();

        // ---------- Optional strings ----------
        if (!macro.empty())
            (new TNamed("macro", macro.c_str()))->Write();

        if (!output.empty())
            (new TNamed("output", output.c_str()))->Write();

        if (!particle_name.empty())
            (new TNamed("particle_name", particle_name.c_str()))->Write();

        // ---------- Numerical parameters ----------
        (new TParameter<double>("particle_energy_GeV", particle_energy_GeV))->Write();
        (new TParameter<int>("nevents", nevents))->Write();
        (new TParameter<int>("nthreads", nthreads))->Write();
        (new TParameter<double>("f_ECAL", f_ECAL))->Write();
        (new TParameter<double>("f_HCAL", f_HCAL))->Write();

        // ---------- Boolean ----------
        (new TParameter<bool>("vis_mode", vis_mode))->Write();
        (new TParameter<bool>("airECAL", airECAL))->Write();
        (new TParameter<bool>("visSensitiveOnly", visSensitiveOnly))->Write();

        // ---------- Geant4 version ----------
        (new TNamed("geant4_version", G4Version))->Write();

        d->cd();
    }

    void SaveToROOTfile(std::string ofilename) const
    {
        TFile * ofile = TFile::Open( ofilename.c_str() , "update");
        this->Write(ofile);
        ofile->Close();
    }
#endif
    // -------- Inline operator<< --------
    friend std::ostream& operator<<(std::ostream& os, const YourInputArgs& a) {
        os << "Configuration:\n";

        os << "  Geometry         : " << a.geometry << "\n";
        os << "  Physics list     : " << a.physics_list << "\n";
        os << "  Macro file       : " << a.macro << "\n";
        os << "  Output file      : " << a.output << "\n";

        os << "  Particle         : " << a.particle_name << "\n";
        os << "  Energy [GeV]     : "
               << std::fixed << std::setprecision(3)
               << a.particle_energy_GeV << "\n";

        os << "  N events         : " << a.nevents << "\n";

        os << "  Visualization    : "
           << (a.vis_mode ? "ON" : "OFF") << "\n";

        os << "  Air ECAL         : " << (a.airECAL ? "TRUE" : "FALSE") << "\n";
        os << "  Vis Sensitive    : " << (a.visSensitiveOnly ? "TRUE" : "FALSE") << "\n";
        os << "  Number threads   : " << a.nthreads << "\n";
        os << "  Energy calibration factors ECAL/HCAL : "
               << std::fixed << std::setprecision(3)
               << a.f_ECAL << " / " << a.f_HCAL << "\n";

        return os;
    }

};

#endif
