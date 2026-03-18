#ifndef PhysicsList_hh
#define PhysicsList_hh

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class CMSPhysicsList : public G4VModularPhysicsList
{
  public:
    CMSPhysicsList();
    ~CMSPhysicsList() override = default;
    static void SetCMSParameters();

  public:
    void SetCuts() override;
};

#endif
