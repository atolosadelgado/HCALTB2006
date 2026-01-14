#include "YourInputArgsParser.hh"
#include <iostream>
#include <cstdlib>

YourInputArgParser::YourInputArgParser(int argc, char** argv)
    : argc_(argc), argv_(argv)
{}

bool YourInputArgParser::parse()
{
    if (argc_ == 1) {
        help(argv_[0]);
        return false;
    }

    for (int i = 1; i < argc_; ++i) {
        if (!parse_one(i))
            return false;
    }

    // ---- Required arguments ----
    if (args_.geometry.empty() || args_.physics_list.empty()) {
        std::cerr << "ERROR: -g and -pl are required\n";
        return false;
    }

    // ---- Cross-checks ----
    if (args_.particle_energy_GeV >= 0.0 && args_.particle_name.empty()) {
        std::cerr << "ERROR: -penergy requires -pname\n";
        return false;
    }

    if (!args_.particle_name.empty() && args_.particle_energy_GeV < 0.0) {
        std::cerr << "ERROR: -pname requires -penergy\n";
        return false;
    }

    return true;
}

bool YourInputArgParser::parse_one(int& i)
{
    std::string arg = argv_[i];

    auto need_value = [&](const std::string& opt) {
        if (i + 1 >= argc_) {
            std::cerr << "Missing value for " << opt << "\n";
            return false;
        }
        return true;
    };

    if (arg == "-h" || arg == "--help") {
        help(argv_[0]);
        std::exit(0);
    }
    else if (arg == "-g") {
        if (!need_value(arg)) return false;
        args_.geometry = argv_[++i];
    }
    else if (arg == "-pl") {
        if (!need_value(arg)) return false;
        args_.physics_list = argv_[++i];
    }
    else if (arg == "-m") {
        if (!need_value(arg)) return false;
        args_.macro = argv_[++i];
    }
    else if (arg == "-o") {
        if (!need_value(arg)) return false;
        args_.output = argv_[++i];
    }
    else if (arg == "-penergy") {
        if (!need_value(arg)) return false;
        args_.particle_energy_GeV = std::stod(argv_[++i]);
    }
    else if (arg == "-pname") {
        if (!need_value(arg)) return false;
        args_.particle_name = argv_[++i];
    }
    else if (arg == "-nevents") {
        if (!need_value(arg)) return false;
        args_.nevents = std::stoi(argv_[++i]);
    }
    else if (arg == "-saturation") {
        if (!need_value(arg)) return false;
        args_.saturation = std::stoi(argv_[++i]);
    }
    else if (arg == "-vis") {
        args_.vis_mode = true;
    }
    else if (arg == "-novis") {
        args_.vis_mode = false;
    }
    else if (arg == "-airECAL") {
        args_.airECAL = true;
    }
    else {
        std::cerr << "Unknown option: " << arg << "\n";
        return false;
    }

    return true;
}

const YourInputArgs& YourInputArgParser::args() const
{
    return args_;
}

void YourInputArgParser::help(const char* prog)
{
    std::cout
        << "Usage:\n"
        << "  " << prog << " -g <geometry.gdml> -pl <physics_list> [options]\n\n"
        << "Required:\n"
        << "  -g         Geometry GDML file\n"
        << "  -pl        Physics list name\n\n"
        << "Optional:\n"
        << "  -m         Macro file\n"
        << "  -o         Output filename\n"
        << "  -penergy   Particle energy (MeV)\n"
        << "  -pname     Particle name (e.g. pi-)\n"
        << "  -nevents   Number of events (0 = macro/UI)\n"
        << "  -vis       Enable visualization\n"
        << "  -novis     Disable visualization (default)\n"
        << "  -s         Saturation effect model; 0 = none, 1 = Geant4 Birk, 2 = CMS Birk \n"
        << "  -airECAL   Make ECAL of air\n"
        << "  -h         Show this help\n";
}
