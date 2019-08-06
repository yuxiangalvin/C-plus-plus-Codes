#include "circle_lib.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
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
    cout << result;
    return 0;
}
