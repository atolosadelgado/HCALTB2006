#include "YourClusterCut.hh"

#include "YourEventInfo.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

bool YourClusterCut::IsInside(const G4Step* aStep) const
{
    YourEventInfo* evtInfo =
        dynamic_cast<YourEventInfo*>(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());

    double hit_radius = Calculate_hitpos_to_shower_axis_distance(aStep, evtInfo->primaryDir, evtInfo->primaryPos);

    return (hit_radius<fRadialCut);
}

double YourClusterCut::Calculate_hitpos_to_shower_axis_distance(const G4Step* aStep, G4ThreeVector & dir0,G4ThreeVector & pos0)
{
    auto & prepos = aStep->GetPreStepPoint()->GetPosition();
    auto & postpos = aStep->GetPostStepPoint()->GetPosition();
    auto  hit_averaged_position = 0.5*(prepos + postpos);

    // Solution distance from a point to a line given here:
    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Vector_formulation
    G4ThreeVector & n = dir0;
    G4ThreeVector & a = pos0;

    G4ThreeVector a_minus_p  = a - hit_averaged_position;
    double   a_minus_p_dot_n = a_minus_p.dot(n);
    G4ThreeVector scaled_n   = a_minus_p_dot_n * n;
    G4ThreeVector hit_to_axis_vector = a_minus_p - scaled_n;
    return hit_to_axis_vector.mag();

}
