#include "carbonTracker.hpp"
#include <iostream>     
#include <cassert> 

using namespace std;

bool sameCTArrays(double* arr1, double* arr2){
    bool sameArrays = true;
    for(int i = 0; i< CarbonTracker::LAST; ++i){
        if(arr1[i] != arr2[i]){
            sameArrays = false;
        }
        
    }
    return sameArrays;
}

void testCorrectConstructor(){
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
}

void testWrongConstructor(){
    cout<<"Error Simple Parameterized Array Tests"<<endl;
    Hector::unitval carbon(10, Hector::U_UNITLESS);
    CarbonTracker soil1(carbon, CarbonTracker::SOIL);
}

void testCopyConstructor(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker ct1(carbon10, CarbonTracker::SOIL);
    CarbonTracker ct2(ct1);
    double* originFracs1 = ct1.getOriginFracs();
    double* originFracs2 = ct2.getOriginFracs();

    cout<<"Copy Constructor Test" <<endl;
    H_ASSERT(ct1.getTotalCarbon() == ct2.getTotalCarbon(), "Copy constructor total carbon doesn't work");
    H_ASSERT(sameCTArrays(originFracs1, originFracs2), "Copy constructor arrays don't work");
}

void testAssignmentOperator(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker ct1(carbon10, CarbonTracker::SOIL);
    Hector::unitval carbon20(20, Hector::U_PGC);
    CarbonTracker ct2(carbon20, CarbonTracker::SOIL);
    ct2=ct1;
    double* originFracs1 = ct1.getOriginFracs();
    double* originFracs2 = ct2.getOriginFracs();

    cout<<"Assignment Operator Test" <<endl;
    H_ASSERT(ct1.getTotalCarbon() == ct2.getTotalCarbon(), "Assignment Operator total carbon doesn't work");
    H_ASSERT(sameCTArrays(originFracs1, originFracs2), "Assignment Operator arrays don't work");
}

void testAddOperator(){
    Hector::unitval carbon1(10, Hector::U_PGC);
    CarbonTracker ct1(carbon1, CarbonTracker::SOIL);
    Hector::unitval carbon2(10, Hector::U_PGC);
    CarbonTracker ct2(carbon2, CarbonTracker::ATMOSPHERE);

    Hector::unitval carbon3(20, Hector::U_PGC);
    double arr[] = {0.5, 0.5, 0, 0};
    CarbonTracker ct3 = ct1 + ct2;

    cout<< "Add Operator Test" << endl;
    H_ASSERT(ct3.getTotalCarbon() == carbon3, "total carbon doesn't add right");
    H_ASSERT(sameCTArrays(ct3.getOriginFracs(), arr), "Arrays don't add right");
}



int main(int argc, char* argv[]){
    cout << "Time for Tests!" << endl;
    testCorrectConstructor();
    //testWrongConstructor();
    testCopyConstructor();
    testAssignmentOperator();
    testAddOperator();
}

