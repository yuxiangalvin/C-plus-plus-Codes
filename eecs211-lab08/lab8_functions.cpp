#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Vehicle.h"
#include "Car.h"
#include "Plane.h"
#include "Boat.h"

using namespace std;

/// Move the vehicle until its position has changed by the specified distance (miles)
/// Return the number of half-hours it took for the vehicle's position to be changed by the specified distance
int move_for_distance(shared_ptr<Vehicle> v, double distance){
    int count = 0;
    while (!((*v).position() >= distance)){
        (*v).Drive();
        count ++;
    }
    return count;
}

/// Move the vehicle until the specified amount of time has passed (hours)
/// Returns the distance covered by vehicle in the specified amount of time
double move_for_time(shared_ptr<Vehicle> v, double time){
    double drive_time = 0;
    while (drive_time < time){
        (*v).Drive();
        drive_time += DELTA_TIME;
    }
    return drive_time;
}

/// Race a set of vehicles over a set distance. The function takes in a vector of Vehicle shared_ptrs
/// Returns the shared_ptr to the Vehicle which wins the race,
/// i.e. the vehicle that moves the distance in the least amount of time
shared_ptr<Vehicle> race(vector<shared_ptr<Vehicle>> vehicles, double distance) {
    shared_ptr<Vehicle> fastest = nullptr;
    int time = 100000;
    for (int i = 0; i < vehicles.size(); i++){
        if (time > move_for_distance(vehicles[i], distance)) {
            time = move_for_distance(vehicles[i], distance);
            fastest = vehicles[i];
        }
    }
    return fastest;
}





