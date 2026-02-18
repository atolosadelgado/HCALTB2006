#ifndef CMSHadronPhysicsFTFP_BERT_hh
#define CMSHadronPhysicsFTFP_BERT_hh

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class CMSHadronPhysicsFTFP_BERT : public G4VPhysicsConstructor
{
  public:
    CMSHadronPhysicsFTFP_BERT(double minFTFP, double maxBERT, double maxBERTpi, double minFTFP_2, double maxBERT_2);
    ~CMSHadronPhysicsFTFP_BERT() override = default;
    void ConstructParticle() override {}
    void ConstructProcess() override {};
};

#endif
