#ifndef ECalSD_config_h
#define ECalSD_config_h

#include <string>
#include <vector>
#include <cstdint>

/* ECalSD_config struct gathers all the members of CMSSW classes:
 *   - ECalSD
 *   - CaloSD (mother of ECalSD)
 *   - EcalSimulationParameters (external class)
 * Numbering schema is not saved here
 */
struct ECalSD_config {
public:
    struct Vec3 {
    double x = {0.0};
    double y = {0.0};
    double z = {0.0};
    };

    struct UnitIDInfo {
    uint32_t rawid = {0};
    int depth      = {0};
    double time    = {0.0};
    int trackID    = {0};
    };
  // raw pointers
  // void* ecalSimParameters = {reinterpret_cast<void*>(0x7f13aa080580)};
  // void* numberingScheme  = {reinterpret_cast<void*>(0x7f137e846540)};

  // bools
  bool useWeight           = {true};
  bool storeTrack          = {false};
  bool storeRL             = {false};
  bool storeLayerTimeSim   = {false};

  bool useBirk             = {true};
  bool useBirkL3           = {true};

  // doubles
  double birk1      = {2.08029e+18};
  double birk2      = {0.0};
  double birk3      = {1.0};
  double birkSlope  = {0.253694};
  double birkCut    = {0.1};

  double slopeLY    = {0.02};
  double scaleRL    = {1.0};

  // strings
  std::string crystalMat  = {"E_PbWO4"};
  std::string depth1Name  = {""};
  std::string depth2Name  = {""};

  // size of containers
  size_t xtalLMapSize   = {34};
  size_t useDepth1Size  = {0};
  size_t useDepth2Size  = {0};
  size_t noWeightSize   = {0};

  // runtime cache
  Vec3 currentLocalPoint = {-0.650662, 9.12936, 83.5814};

  double crystalLength  = {230.0};
  // calculated for each hit
  // double crystalDepth   = {31.4186};
  int depth             = {0};

  bool ageingWithSlopeLY = {false};
  // external clases called within EcalSD
    struct EcalSimulationParameters {
    public:
    int nxtalEta  = {85};
    int nxtalPhi  = {360};
    int phiBaskets = {18};

    std::vector<int> etaBaskets = {25, 45, 65, 85};

    int ncrys = {0};
    int nmods = {0};

    bool useWeight = {true};

    std::string depth1Name = {""};
    std::string depth2Name = {""};

    std::vector<std::string> lvNames = {
        "EBRY_01", "EBRY_02", "EBRY_03", "EBRY_04", "EBRY_05",
        "EBRY_06", "EBRY_07", "EBRY_08", "EBRY_09", "EBRY_10",
        "EBRY_11", "EBRY_12", "EBRY_13", "EBRY_14", "EBRY_15",
        "EBRY_16", "EBRY_17"
    };

    std::vector<std::string> matNames = {
        "E_PbWO4", "E_PbWO4", "E_PbWO4", "E_PbWO4", "E_PbWO4",
        "E_PbWO4", "E_PbWO4", "E_PbWO4", "E_PbWO4", "E_PbWO4",
        "E_PbWO4", "E_PbWO4", "E_PbWO4", "E_PbWO4", "E_PbWO4",
        "E_PbWO4", "E_PbWO4"
    };

    std::vector<double> dzs = {
        230, 230, 230, 230, 230, 230, 230, 230, 230,
        230, 230, 230, 230, 230, 230, 230, 230
    };
    };
    class CaloSD_config {
    public:
    // posiciones
    Vec3 entrancePoint = {1284.6, -169.221, 284.077};
    Vec3 entranceLocal = {1.20282, 10.8182, 115.0};
    Vec3 posGlobal     = {1284.6, -169.221, 284.077};

    // energías
    double incidentEnergy = {29055.8};
    double edepositEM     = {0.0};
    double edepositHAD    = {0.0};

    // IDs
    UnitIDInfo currentID[2] = {
        {0x32011a0d, 0, 31.3381, 1},
        {0x0,        0, -2.0,   -2}
    };

    UnitIDInfo previousID[2] = {
        {0x32011a0d, 0, 31.2302, 1},
        {0x0,        0, -2.0,   -2}
    };

    // thresholds
    double energyCut = {1000.0};
    double tmaxHit   = {1000.0};
    double eminHit   = {0.0};

    // flags
    bool suppressHeavy = {false};

    int kmaxIon     = {30};
    int kmaxNeutron = {30};
    int kmaxProton  = {30};

    bool forceSave = {false};
    int nHC        = {1};

    std::string detName   = {"EcalHitsEB"};
    std::string collName0 = {"EcalHitsEB"};
    std::string collName1 = {""};

    bool ignoreTrackID   = {false};
    bool isParameterized = {false};
    bool ignoreReject    = {false};

    bool useMap        = {false};
    bool corrTOFBeam   = {false};

    int timeSlice      = {1};
    double eminHitD    = {0.0};
    bool correctT      = {false};

    bool doFineCalo           = {false};
    double eMinFine           = {10000.0};
    bool doFineCaloThisStep   = {false};

    std::vector<std::string> hcn_ = {
        "EcalHitsEB", "EcalHitsEE", "EcalHitsES", "HcalHits", "ZDCHITS"
    };

    std::vector<int> useResMap_ = {0, 0, 0, 0, 0};
    };
  EcalSimulationParameters EcalSim_parameters;
  CaloSD_config            CaloSD_parameters;

  // LV attached to ECalSD (dumped from CMSSW)
  std::vector<std::string> sensitive_lv = { "EBRY_01_refl",
                                            "EBRY_01",
                                            "EBRY_02",
                                            "EBRY_02_refl",
                                            "EBRY_03",
                                            "EBRY_03_refl",
                                            "EBRY_04",
                                            "EBRY_04_refl",
                                            "EBRY_05",
                                            "EBRY_05_refl",
                                            "EBRY_06",
                                            "EBRY_06_refl",
                                            "EBRY_07",
                                            "EBRY_07_refl",
                                            "EBRY_08",
                                            "EBRY_08_refl",
                                            "EBRY_09",
                                            "EBRY_09_refl",
                                            "EBRY_10",
                                            "EBRY_10_refl",
                                            "EBRY_11",
                                            "EBRY_11_refl",
                                            "EBRY_12",
                                            "EBRY_12_refl",
                                            "EBRY_13",
                                            "EBRY_13_refl",
                                            "EBRY_14",
                                            "EBRY_14_refl",
                                            "EBRY_15",
                                            "EBRY_15_refl",
                                            "EBRY_16",
                                            "EBRY_16_refl",
                                            "EBRY_17",
                                            "EBRY_17_refl"
                                        };
};


#endif
