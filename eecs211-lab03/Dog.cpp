#include "Dog.h"

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

Dog youngestDog(vector<Dog> dogs)
{
    for (size_t i=1;i < dogs.size(); ++i){
        if (dogs[i].age<dogs[0].age){
                dogs[0]=dogs[i];
        }
    }
    return dogs[0];
}

Dog happiestDog(vector<Dog> dogs)
{
    for (size_t i=1;i < dogs.size(); ++i){
        if (dogs[i].happiness>dogs[0].happiness){
            dogs[0]=dogs[i];
        }
    }
    return dogs[0];
}

string findOwner(const vector<Dog> dogs, const string name)
{
    Dog result;
    for (size_t i=0;i < dogs.size(); ++i) {
        if (dogs[i].name == name){
            result=dogs[i];
        }
    }
    if (result.name=="") {
        cerr << "No dog has this name\n";
        throw runtime_error("Message for the error");}

    return result.owner;
}



