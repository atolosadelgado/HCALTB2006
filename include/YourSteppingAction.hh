#ifndef YOURSTEPPINGACTION_HH
#define YOURSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include <memory>

class YourEventAction;
class G4Region;
class G4Material;
class YourVEMSaturation;

class YourSteppingAction : public G4UserSteppingAction {
  public:
    YourSteppingAction(YourEventAction* evtAction);
    ~YourSteppingAction() override;

    void UserSteppingAction(const G4Step* step) override;
    void SetEcalRegion(G4Region * r){fEcalRegion = r;}
    void SetHcalRegion(G4Region * r){fHcalRegion = r;}

    void SetEcalSensMat(G4Material * m){fEcalMat = m;}
    void SetHcalSensMat(G4Material * m){fHcalMat = m;}


    void SetSaturationNone();
    void SetSaturationG4Birk();
    void SetSaturationCMSBirk();

  private:

    YourEventAction*             fYourEventAction;
    G4Region * fEcalRegion;
    G4Region * fHcalRegion;
    G4Material * fEcalMat;
    G4Material * fHcalMat;
    // general object that may apply G4 Birk quenching, CMS quadratic quenching or none
    std::unique_ptr<YourVEMSaturation> fHCAL_emsaturation;
};

#endif
