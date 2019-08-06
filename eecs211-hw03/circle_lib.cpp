#include "circle_lib.h"

#include <iostream>
#include <stdexcept>
#include <math.h>

using namespace std;

Circle read_circle()
{ 
    Circle o;
    cin>> o.x>> o.y>> o.radius;
    return o;
}

bool overlapped(Circle c1, Circle c2)
{
    //check whether radius are positive
    if ((c1.radius < 0) || (c2.radius < 0)){
        throw runtime_error("radius must be non-negative")
    }

    double distance = sqrt(pow((c2.x - c1.x), 2) + pow((c2.y - c1.y), 2));
    double radius_sum = c1.radius + c2.radius;

    if (distance < radius_sum) {
        return true;
    }
    else{
        return false;
    }
}

