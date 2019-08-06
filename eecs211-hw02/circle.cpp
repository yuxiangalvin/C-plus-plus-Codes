#include <iostream>
#include <math.h>
#include <string>

using namespace std;

// Represents a circle on the plane.
struct Circle
{
    double x, y;          // (x, y) is the center
    double radius;
};

// Reads a circle from cin and returns it.
Circle read_circle()
{ 
    Circle o;
    cin>> o.x>> o.y>> o.radius;
    return o;
}

// Determines whether two circles overlap.
bool overlapped(Circle c1, Circle c2)
{
    bool b = false;

    if ((c1.radius+c2.radius)>sqrt((c2.x-c1.x)*(c2.x-c1.x)+(c2.y-c1.y)*(c2.y-c1.y))){
        b = true;
    }
    return b;
}

int main() {
    string result;
    Circle C = read_circle();
    while (true){
        Circle C_i = read_circle();
        if (C_i.radius<0){
            break;
        }
        if (overlapped(C, C_i)){   
            result += "overlapped\n";
        }
        else {
            result += "not overlapped\n";
        }
    }
    result.erase(result.length()-1);
    cout << result;
    return 0;
}