#ifndef YOURPARTICLEINFO_HH
#define YOURPARTICLEINFO_HH

#include <map>

struct YourParticleInfo {
  int pdg;
  int hIDe0;
  int hIDef;
  int hIDtf;
  static constexpr int PDG_OTHER = 0;
};

using YourParticleInfoMap = std::map<int, YourParticleInfo>;

#endif
