#ifndef YOURSTEPPINGACTION_HH
#define YOURSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class YourEventAction;
class G4VPhysicalVolume;

class YourSteppingAction : public G4UserSteppingAction {
  public:
   YourSteppingAction(YourEventAction* evtAction);
    ~YourSteppingAction() override;
    void UserSteppingAction(const G4Step* step) override;

    void SetCaloPV(G4VPhysicalVolume * caloLV){fCaloPV = caloLV;}
    void SetTBHCalPV(G4VPhysicalVolume * TBHCalPV ){fTBHCalPV = TBHCalPV;}

  private:
    YourEventAction*  fYourEventAction = {nullptr};
    G4VPhysicalVolume * fCaloPV = {nullptr};
    G4VPhysicalVolume * fTBHCalPV = {nullptr};
    void UpdateLeakedEnergy(const G4Step * step);
};

#endif
