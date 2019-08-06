#include <iostream>

using namespace std;

// Determines whether the given number is prime.
bool is_prime(int p)
{bool b = true;
    for (int i=2; i<p; ++i){
    if (p % i ==0){
    b=false;
    break;
    }
    }
    return b;
}

int main()
{
  int n;
  cin>>n;
  for (int i=2; i<=n; ++i){
    if (is_prime(i)){
      cout<<i<<"\n";
    }
  }
}
