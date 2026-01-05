#ifndef YOURSTEPPINGACTION_HH
#define YOURSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class YourEventAction;
class G4Region;

class YourSteppingAction : public G4UserSteppingAction {
  public:
    YourSteppingAction(YourEventAction* evtAction);
    ~YourSteppingAction() override;

    void UserSteppingAction(const G4Step* step) override;
    void SetEcalRegion(G4Region * r){ecal_region = r;}
    void SetHcalRegion(G4Region * r){hcal_region = r;}

  private:

    YourEventAction*             fYourEventAction;
    G4Region * ecal_region;
    G4Region * hcal_region;
};

#endif
