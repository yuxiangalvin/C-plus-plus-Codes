#include "prime_lib.h"

#include <stdexcept>
#include <vector>

using namespace std;

// check whether a number p is a prime number by dividing p by all integers between 2 and p − 1;
bool is_prime(int p){
    bool b = true;

    for (int i = 2; i < p; ++i){
        if (p % i == 0){
        b = false;
        break;
        }
    }
    return b;
}

// check whether a number p is a prime number by dividing p by the primes between 2 and p − 1;
bool check_is_prime(const std::vector<int>& primes, int p){

    if (p < 1){
        throw runtime_error("number needs to be positive.\n");
    }

    if (primes.empty()){
        throw runtime_error("primes can not be empty.\n");
    }

    if (p == 1){
        return false;
    }

    for (size_t i = 0; (i < p) && (i < primes.size()); i++){
        if (primes[i] == p){
        return true;
    }
        if (p % primes[i] == 0){
        return false;
        }
    }
    return true;
}

// uses the sieve of Eratosthenes to generate all primes between 2 and n；
vector<int> generate_primes(int n)
{
    vector<bool> sieve(n + 1);
    vector<int> primes;

    for (size_t i = 2; i <= n; i++){
        sieve[i] = true;
    }

    for (size_t i = 2; i <= n; i++){
        if (sieve[i]){
            for (size_t ii = i + i; ii <= n; ii = ii + i){
                (sieve[ii]) = false;
            }
        }
    }

    for (size_t i = 2; i <= n; ++i){
        if (sieve[i]){
        primes.push_back(i);
        }
    }
    return primes;
}

