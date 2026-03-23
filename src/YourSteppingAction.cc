#include "YourSteppingAction.hh"

#include "YourEventAction.hh"

#include "G4Step.hh"

YourSteppingAction::YourSteppingAction(YourEventAction* evtAction)
:   G4UserSteppingAction(),
    fYourEventAction(evtAction) { }


YourSteppingAction::~YourSteppingAction() {}

void YourSteppingAction::UserSteppingAction(const G4Step* theStep) {

    const G4double eDep = theStep->GetTotalEnergyDeposit();
    if(eDep<=0.0) return;
    G4LogicalVolume * lv = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();

    double radius = Calculate_hitpos_to_shower_axis_distance(theStep);
    fYourEventAction->UpdateProfileHistograms(lv, eDep, radius);
}

double YourSteppingAction::Calculate_hitpos_to_shower_axis_distance(const G4Step* aStep) const
{
    auto & prepos = aStep->GetPreStepPoint()->GetPosition();
    auto & postpos = aStep->GetPostStepPoint()->GetPosition();
    auto  hit_averaged_position = 0.5*(prepos + postpos);
    auto & primary_VertexDir= fYourEventAction->GetPrimaryVertexDir();
    auto & primary_VertexPos = fYourEventAction->GetPrimaryVertexPos();

    // Solution distance from a point to a line given here:
    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Vector_formulation
    G4ThreeVector & n = primary_VertexDir;
    G4ThreeVector & a = primary_VertexPos;

    G4ThreeVector a_minus_p  = a - hit_averaged_position;
    double   a_minus_p_dot_n = a_minus_p.dot(n);
    G4ThreeVector scaled_n   = a_minus_p_dot_n * n;
    G4ThreeVector hit_to_axis_vector = a_minus_p - scaled_n;
    return hit_to_axis_vector.mag();

}
