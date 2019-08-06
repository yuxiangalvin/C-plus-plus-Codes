#include "Boat.h"

using namespace std;

/// Update the position_ data member inherited from Vehicle
/// Give the boat a speed of 20mph if its movementType_ data member is "Sail Boat"
///     or a speed of 50mph if movementType_ is "Motor Boat"
/// Recall that distance = speed * time
void Boat::Drive() {

    if (movementType_ == "Sail Boat"){
        position_ += 20 * DELTA_TIME;
    } else {
        position_ += 50 * DELTA_TIME;
    }

}

string Boat::vehicleType() {
    return "Boat";
}


