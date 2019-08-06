#include "Car.h"
using namespace std;

/// Update the position_ data member inherited from Vehicle
/// Make the car move forward proportional to its speed_ data member
/// Recall that distance = speed * time
void Car::Drive() {

    position_ += speed_*DELTA_TIME;

}

string Car::vehicleType() {
    return "Car";
}