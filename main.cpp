#include "carbonTracker.hpp"
#include <iostream>     
#include <cassert> 

using namespace std;

bool sameCTArrays(double* arr1, double* arr2){
    bool sameArrays = true;
    for(int i = 0; i< CarbonTracker::LAST; ++i){
        if(arr1[i] != arr2[1]){
            sameArrays = false;
        }
        
    }
    return sameArrays;
}

bool testCorrectConstructors(){
    Hector::unitval carbon(10, Hector::U_PGC);
    CarbonTracker soil1(carbon, CarbonTracker::SOIL);
    double* originFracs1 = soil1.getOriginFracs();
    bool correctArray = true;
    for(int i = 0; i< CarbonTracker::LAST; ++i){
        if(originFracs1[i] != 0 && i != CarbonTracker::SOIL){
            correctArray = false;
        }
        if(i==CarbonTracker::SOIL && originFracs1[i] != 1){
            correctArray = false;
        }
    }
    cout<<"Simple Parameterized Array Tests"<<endl;
    H_ASSERT(soil1.getTotalCarbon() == 10, "Wrong Carbon Amount Simple Constructor");
    H_ASSERT(correctArray, "Wrong Carbon Fraction Array Simple Constructor")

    // double setOriginFracs2[4] = {0.3, 0.4, 0.1, 0.2};
    // CarbonTracker soil2(carbon, setOriginFracs2);
    // double* getOriginFracs2 = soil2.getOriginFracs();
    // H_ASSERT(soil1.getTotalCarbon() == 10, "Wrong Carbon Amount Complicated Constructor");
    // cout<<sameCTArrays(setOriginFracs2, getOriginFracs2)<<endl;
    // H_ASSERT(sameCTArrays(setOriginFracs2, getOriginFracs2), "Wrong Carbon Fraction Array Complicated Constructor");
}


int main(int argc, char* argv[]){
    cout << "Time for Tests!" << endl;
}

