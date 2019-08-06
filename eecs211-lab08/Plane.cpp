#include "Plane.h"

using namespace std;

/// Update the position_ data member inherited from Vehicle
/// Give the plane 100mph of speed for each engine it has
/// Recall that distance = speed * time
void Plane::Drive() {

    position_ += engines_*100*DELTA_TIME;


}

string Plane::vehicleType() {
    return "Plane";
}

