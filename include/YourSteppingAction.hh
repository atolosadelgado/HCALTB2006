#ifndef YOURSTEPPINGACTION_HH
#define YOURSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class YourEventAction;
class G4Region;
class G4Material;

class YourSteppingAction : public G4UserSteppingAction {
  public:
    YourSteppingAction(YourEventAction* evtAction);
    ~YourSteppingAction() override;

    void UserSteppingAction(const G4Step* step) override;
    void SetEcalRegion(G4Region * r){ecal_region = r;}
    void SetHcalRegion(G4Region * r){hcal_region = r;}

    void SetEcalSensMat(G4Material * m){ecal_sensitivemat = m;}
    void SetHcalSensMat(G4Material * m){hcal_sensitivemat = m;}

  private:

    YourEventAction*             fYourEventAction;
    G4Region * ecal_region;
    G4Region * hcal_region;
    G4Material * ecal_sensitivemat;
    G4Material * hcal_sensitivemat;
};

#endif
