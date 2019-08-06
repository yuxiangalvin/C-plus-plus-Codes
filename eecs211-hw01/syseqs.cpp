#include <iostream>

using namespace std;

int main() {
    /*
     Coefficients of the system
     ax + by = c
     dx + ey = f
     */
    int a, b, c, d, e, f;
    
    cin >>a >> b >> c >> d >> e >> f;
    
    double x = ((c*e-f*b)/(a*e-d*b));
    double y = ((a*f-d*c)/(a*e-d*b));
    
    cout << x << "\n";
    cout << y << "\n";
    
    /*
     TODO: Write your code here
     
     1. Read input by
     cin >> ......;
     2. Solve for x and y
     3. Print x and y by
     cout << SOLUTION_OF_X << "\n";
     cout << SOLUTION_OF_Y << "\n";
     */
}
