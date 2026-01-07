#ifndef YOURINPUTARGS
#define YOURINPUTARGS

#include <string>
#include <ostream>
#include <iomanip> // std::fixed, std::setprecision

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

    // -------- Inline operator<< --------
    friend std::ostream& operator<<(std::ostream& os, const YourInputArgs& a) {
        os << "Configuration:\n";

        os << "  Geometry        : " << a.geometry << "\n";
        os << "  Physics list    : " << a.physics_list << "\n";
        os << "  Macro file      : " << a.macro << "\n";
        os << "  Output file     : " << a.output << "\n";

        os << "  Particle        : " << a.particle_name << "\n";
        os << "  Energy [GeV]    : "
               << std::fixed << std::setprecision(3)
               << a.particle_energy_GeV << "\n";

        os << "  N events        : " << a.nevents << "\n";

        os << "  Visualization   : "
           << (a.vis_mode ? "ON" : "OFF") << "\n";

        return os;
    }
};

#endif
