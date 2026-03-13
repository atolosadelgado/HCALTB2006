#ifndef YourEventAction_h
#define YourEventAction_h

#include <limits>
#include <vector>

#include "G4UserEventAction.hh"

class YourPrimaryGenerator;

class YourEventAction : public G4UserEventAction {
public:
  void EndOfEventAction(const G4Event*) override;
  void BeginOfEventAction(const G4Event*) override;
  void SetPrimaryGenerator(YourPrimaryGenerator * p){fPrimaryGenerator=p;}
  void UpdateTime(double t);
  double GetEventTime(){return time_last_hit - time_first_hit; }
  void ResetEventTime(){
    time_first_hit = {std::numeric_limits<double>::max()};
    time_last_hit = {-std::numeric_limits<double>::max()};
  };

    std::vector<double> fGammaE0_ecal;
    std::vector<int> fGammaModelIndex_ecal;
    std::vector<double> fNeutronEfinal;
    std::vector<double> fNeutronTfinal;
private:
    YourPrimaryGenerator * fPrimaryGenerator;
    double time_first_hit = {std::numeric_limits<double>::max()};
    double time_last_hit = {-std::numeric_limits<double>::max()};
};

#endif
