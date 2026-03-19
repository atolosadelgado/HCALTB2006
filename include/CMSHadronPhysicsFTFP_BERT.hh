#ifndef CMSHadronPhysicsFTFP_BERT_hh
#define CMSHadronPhysicsFTFP_BERT_hh

#include "globals.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"

class CMSHadronPhysicsFTFP_BERT : public G4HadronPhysicsFTFP_BERT {
public:
  explicit CMSHadronPhysicsFTFP_BERT(G4int verb);
  explicit CMSHadronPhysicsFTFP_BERT(G4double e1, G4double e2, G4double e3, G4double e4, G4double e5);
  ~CMSHadronPhysicsFTFP_BERT() override = default;

  void ConstructProcess() override;

  void Neutron() override;

  // copy constructor and hide assignment operator
  CMSHadronPhysicsFTFP_BERT(CMSHadronPhysicsFTFP_BERT &) = delete;
  CMSHadronPhysicsFTFP_BERT &operator=(const CMSHadronPhysicsFTFP_BERT &right) = delete;

  bool fPrintArguments = {false};
};


#endif
