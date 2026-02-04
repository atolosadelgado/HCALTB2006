#ifndef HCalSD_config_h
#define HCalSD_config_h

#include <string>
#include <vector>
#include <cstdint>

/* HCalSD_config struct gathers all the members of CMSSW classes:
 *   - HCalSD
 *   - CaloSD (mother of HCalSD)
 * Numbering schema is not saved here
 */
struct HCalSD_config {
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


  // ============================================================
  // === HcalSD dump ============================================
  // ============================================================

  // --- raw pointers
  // void* numberingFromDDD  = {reinterpret_cast<void*>(0x7f137e9e52b8)};
  // void* numberingScheme   = {reinterpret_cast<void*>(0x7f137e846550)};
  void* showerLibrary     = {nullptr};
  void* hfshower          = {nullptr};
  void* showerParam       = {nullptr};
  void* showerPMT         = {nullptr};
  void* showerBundle      = {nullptr};

  // void* hcalConstants     = {reinterpret_cast<void*>(0x7f13ab19b500)};
  void* hcalSimConstants  = {nullptr};
  void* m_HBDarkening     = {nullptr};
  void* m_HEDarkening     = {nullptr};
  void* m_HFDarkening     = {nullptr};
  void* m_HcalTestNS      = {nullptr};

  // --- Static constants
  double maxZ     = {10000.0};
  double minRoff  = {-1500.0};
  double maxRoff  = {450.0};
  double slopeHE  = {0.4};

  // --- Flags
  bool isHF             = {false};
  bool agingFlagHB      = {false};
  bool agingFlagHE      = {false};
  bool useBirk          = {true};
  bool useLayerWt       = {false};
  bool useFibreBundle   = {false};
  bool usePMTHit        = {false};
  bool testNumber       = {false};
  bool neutralDensity   = {false};
  bool testNS           = {false};
  bool useHF            = {false};
  bool useShowerLibrary = {false};
  bool useParam         = {false};
  bool applyFidCut      = {true};
  bool dd4hep           = {false};

  // --- Parameters
  double birk1     = {3.74491e+17};
  double birk2     = {0.142};
  double birk3     = {1.75};
  double betaThr  = {0.7};

  double eminHitHB = {0.0};
  double eminHitHE = {0.0};
  double eminHitHO = {0.0};
  double eminHitHF = {0.0};

  double deliveredLumi = {5000.0};
  double weight        = {1.0};
  int depth            = {0};

  // --- Sizes of vectors / containers
  size_t gparSize        = {7};
  size_t hfLevelsSize   = {0};
  size_t hfNamesSize    = {0};
  size_t fibreNamesSize = {0};
  size_t matNamesSize   = {1};
  size_t materialsSize  = {1};

  size_t hfLVSize      = {0};
  size_t fibreLVSize   = {0};
  size_t pmtLVSize     = {0};
  size_t fibre1LVSize  = {0};
  size_t fibre2LVSize  = {0};

  size_t layerWeightsSize = {0};

  // --- Histos
  double hit[9]  = {0,0,0,0,0,0,0,0,0};
  double time[9] = {0,0,0,0,0,0,0,0,0};
  double dist[9] = {0,0,0,0,0,0,0,0,0};

  double hzvem  = {0.0};
  double hzvhad = {0.0};

  // --- Misc
  size_t detNullSize = {0};

  // ============================================================
  // ===== CaloSD dump ==========================================
  // ============================================================

  // --- Positions
  Vec3 entrancePoint = {0.0, 0.0, 0.0};
  Vec3 entranceLocal = {0.0, 0.0, 0.0};
  Vec3 posGlobal     = {0.0, 0.0, 0.0};

  // --- Energies
  double incidentEnergy = {0.0};
  double edepositEM     = {0.0};
  double edepositHAD    = {0.0};

  // --- IDs
  UnitIDInfo currentID[2] = {
    {0x43180c03, 0, 32.9746, 1},
    {0x0,        0, -2.0,   -2}
  };

  UnitIDInfo previousID[2] = {
    {0x0, 0, -2.0, -2},
    {0x0, 0, -2.0, -2}
  };

  // --- Thresholds
  double energyCut = {1000.0};
  double tmaxHit   = {1000.0};
  double eminHit   = {0.0};

  // --- Flags
  bool suppressHeavy = {false};

  int kmaxIon     = {30};
  int kmaxNeutron = {30};
  int kmaxProton  = {30};

  bool forceSave = {false};
  int nHC        = {1};

  std::string detName   = {"HcalHits"};
  std::string collName0 = {"HcalHits"};
  std::string collName1 = {""};

  bool ignoreTrackID   = {false};
  bool isParameterized = {true};
  bool ignoreReject    = {false};

  bool useMap      = {false};
  bool corrTOFBeam = {false};

  int timeSlice   = {1};
  double eminHitD = {0.0};
  bool correctT   = {false};

  bool doFineCalo         = {false};
  double eMinFine         = {10000.0};
  bool doFineCaloThisStep = {false};

  std::vector<std::string> hcn = {
    "EcalHitsEB", "EcalHitsEE", "EcalHitsES", "HcalHits", "ZDCHITS"
  };

  std::vector<int> useResMap = {0, 0, 0, 0, 0};

  // LV attached to HCalSD (dumped from CMSSW)
  std::vector<std::string> sensitive_lv = { "HBScintillatorLayer0In1",
                                            "HBScintillatorLayer0In2",
                                            "HBScintillatorLayer1In1",
                                            "HBScintillatorLayer1In2",
                                            "HBScintillatorLayer2In1",
                                            "HBScintillatorLayer2In2",
                                            "HBScintillatorLayer3In1",
                                            "HBScintillatorLayer3In2",
                                            "HBScintillatorLayer4In1",
                                            "HBScintillatorLayer4In2",
                                            "HBScintillatorLayer5In1",
                                            "HBScintillatorLayer5In2",
                                            "HBScintillatorLayer6In1",
                                            "HBScintillatorLayer6In2",
                                            "HBScintillatorLayer7In1",
                                            "HBScintillatorLayer7In2",
                                            "HBScintillatorLayer8In1",
                                            "HBScintillatorLayer8In2",
                                            "HBScintillatorLayer9In1",
                                            "HBScintillatorLayer9In2",
                                            "HBScintillatorLayer10In1",
                                            "HBScintillatorLayer10In2",
                                            "HBScintillatorLayer11In1",
                                            "HBScintillatorLayer11In2",
                                            "HBScintillatorLayer12In1",
                                            "HBScintillatorLayer12In2",
                                            "HBScintillatorLayer13In1",
                                            "HBScintillatorLayer13In2",
                                            "HBScintillatorLayer14In1",
                                            "HBScintillatorLayer14In2",
                                            "HBScintillatorLayer15In1",
                                            "HBScintillatorLayer15In2",
                                            "HBScintillatorLayer16In1",
                                            "HBScintillatorLayer16In2",
                                            "HEScintillatorPart3Layer02Phi0",
                                            "HEScintillatorPart3Layer02Phi1",
                                            "HEScintillatorPart3Layer03Phi0",
                                            "HEScintillatorPart3Layer03Phi1",
                                            "HEScintillatorPart3Layer04Phi0",
                                            "HEScintillatorPart3Layer04Phi1",
                                            "HEScintillatorPart3Layer05Phi0",
                                            "HEScintillatorPart3Layer05Phi1",
                                            "HEScintillatorPart3Layer06Phi0",
                                            "HEScintillatorPart3Layer06Phi1",
                                            "HEScintillatorPart4Layer07Phi0",
                                            "HEScintillatorPart4Layer07Phi1",
                                            "HEScintillatorPart4Layer08Phi0",
                                            "HEScintillatorPart4Layer08Phi1",
                                            "HEScintillatorPart4Layer09Phi0",
                                            "HEScintillatorPart4Layer09Phi1",
                                            "HEScintillatorPart4Layer10Phi0",
                                            "HEScintillatorPart4Layer10Phi1",
                                            "HEScintillatorPart4Layer11Phi0",
                                            "HEScintillatorPart4Layer11Phi1",
                                            "HEScintillatorPart4Layer12Phi0",
                                            "HEScintillatorPart4Layer12Phi1",
                                            "HEScintillatorPart5Layer13Phi0",
                                            "HEScintillatorPart5Layer13Phi1",
                                            "HEScintillatorPart5Layer14Phi0",
                                            "HEScintillatorPart5Layer14Phi1",
                                            "HEScintillatorPart5Layer15Phi0",
                                            "HEScintillatorPart5Layer15Phi1",
                                            "HEScintillatorPart5Layer16Phi0",
                                            "HEScintillatorPart5Layer16Phi1",
                                            "HEScintillatorPart5Layer17Phi0",
                                            "HEScintillatorPart5Layer17Phi1",
                                            "HEScintillatorPart5Layer18Phi0",
                                            "HEScintillatorPart5Layer18Phi1",
                                            "HTSensLayer0",
                                            "HTSensLayer1",
                                            "HTSensLayer2",
                                            "HTSensLayer3"
                                          }; // end vector sensitive_lv


};


#endif
