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
    CarbonTracker soil(carbon1, CarbonTracker::SOIL);
    Hector::unitval carbon2(10, Hector::U_PGC);
    CarbonTracker atmos(carbon2, CarbonTracker::ATMOSPHERE);

    Hector::unitval carbon3(20, Hector::U_PGC);
    double arr[] = {0.5, 0.5, 0, 0};
    CarbonTracker testCT = soil + atmos;

    cout<< "Add Operator Test" << endl;
    H_ASSERT(testCT.getTotalCarbon() == carbon3, "total carbon doesn't add right");
    H_ASSERT(sameCTArrays(testCT.getOriginFracs(), arr), "Arrays don't add right");
}

void testSubtractOperator(){
    Hector::unitval carbon30(30, Hector::U_PGC);
    CarbonTracker soil(carbon30, CarbonTracker::SOIL);
    Hector::unitval carbon10(10, Hector::U_PGC);

    Hector::unitval carbon20(20, Hector::U_PGC);
    double arr[] = {1, 0, 0, 0};
    CarbonTracker testCT = soil - carbon10;

    cout<< "Subtract Operator Test" << endl;
    H_ASSERT(testCT.getTotalCarbon() == carbon20, "total carbon doesn't add right");
    H_ASSERT(sameCTArrays(testCT.getOriginFracs(), arr), "Arrays don't add right");
}

void testWrongSubtraction(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil(carbon10, CarbonTracker::SOIL);
    Hector::unitval carbon30(30, Hector::U_PGC);

    //Throws an error
    CarbonTracker testCT = soil - carbon30;
}

void testSubtractionWithArray(){
    Hector::unitval carbon30(30, Hector::U_PGC);
    CarbonTracker soil30(carbon30, CarbonTracker::SOIL);
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);

    Hector::unitval carbon20(20, Hector::U_PGC);
    double arr1[] = {1, 0, 0, 0};
    CarbonTracker testCT1 = soil30 - soil10;

    Hector::unitval carbon40(40, Hector::U_PGC);
    CarbonTracker atmos40(carbon40, CarbonTracker::ATMOSPHERE);
    CarbonTracker atmosSoil = atmos40+soil30;
    CarbonTracker testCT2 = atmosSoil - soil10;
    Hector::unitval carbon60(60, Hector::U_PGC);
    double arr2[] = {1.0/3, 2.0/3, 0, 0};

    cout<< "Add Operator Test" << endl;
    H_ASSERT(testCT1.getTotalCarbon() == carbon20, "total carbon doesn't subtract right");
    H_ASSERT(sameCTArrays(testCT1.getOriginFracs(), arr1), "Arrays don't subtract right");
    H_ASSERT(testCT2.getTotalCarbon() == carbon60, "total carbon doesn't subtract right - complicated example");
    H_ASSERT(sameCTArrays(testCT2.getOriginFracs(), arr2), "Arrays don't subtract right - complicated example");

}

void testMultiplication(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    double arr1[] = {1, 0, 0, 0};

    CarbonTracker soil5 = soil10 * 0.5;
    CarbonTracker soil20 = 2 * soil10;

    cout<< "Multiplication Operator Test" << endl;
    H_ASSERT(soil5.getTotalCarbon() == 5, "multiplication ct*double doesn't work");
    H_ASSERT(sameCTArrays(soil5.getOriginFracs(), arr1), "multiplication messes up arrays for ct*double");
    H_ASSERT(soil20.getTotalCarbon() == 20, "multiplication double*ct doesn't work");
    H_ASSERT(sameCTArrays(soil20.getOriginFracs(), arr1), "multiplication messes up arrays for ct*double");
}

void testDivision(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    double arr1[] = {1, 0, 0, 0};

    CarbonTracker soil5 = soil10/2;

    cout<< "Division Operator Test" << endl;
    H_ASSERT(soil5.getTotalCarbon() == 5, "multiplication ct*double doesn't work");
    H_ASSERT(sameCTArrays(soil5.getOriginFracs(), arr1), "multiplication messes up arrays for ct*double");
}
void testDivisionBy0(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    CarbonTracker soil5 = soil10/0;
}

void testSetCarbon(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    Hector::unitval carbon20(20, Hector::U_PGC);
    soil10.setTotalCarbon(carbon20);
    double arr1[] = {1, 0, 0, 0};
    cout<< "setTotalCarbon Test" << endl;
    H_ASSERT(soil10.getTotalCarbon() == 20, "setTotalCarbon Ddesn't work");
    H_ASSERT(sameCTArrays(soil10.getOriginFracs(), arr1), "setTotalCarbon changes the array");
}

void testWrongSetCarbon(){
    Hector::unitval year10(10, Hector::U_YRS);
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    soil10.setTotalCarbon(year10);
}

void testGetTotalCarbon(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    Hector::unitval carbonGetter = soil10.getTotalCarbon();

    cout<< "getTotalCarbon Test" << endl;
    H_ASSERT(carbonGetter == 10, "Amount of carbon from getter is wrong");
    H_ASSERT(carbonGetter.units() == Hector::U_PGC, "unites from total carbon getter are wrong");
}

void testGetOriginFracs(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    Hector::unitval carbon40(40, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    CarbonTracker atmos40(carbon40, CarbonTracker::ATMOSPHERE);
    double soil[] = {1, 0, 0, 0};
    double atmos[] = {0, 1, 0, 0};

    cout<< "getOriginFracs Test" << endl;
    H_ASSERT(sameCTArrays(soil10.getOriginFracs(), soil), "Doesn't get first element of array correctly");
    H_ASSERT(sameCTArrays(atmos40.getOriginFracs(), atmos), "Doesn't get second (past first) element of array correctly");
}

void testGetPoolCarbon(){
    Hector::unitval carbon10(10, Hector::U_PGC);
    Hector::unitval carbon40(40, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    CarbonTracker atmos40(carbon40, CarbonTracker::ATMOSPHERE);
    
    cout<< "getPoolCarbon Test" << endl;
    H_ASSERT(soil10.getPoolCarbon(CarbonTracker::SOIL) == 10, "Doesn't get first element's value correctly");
    H_ASSERT(soil10.getPoolCarbon(CarbonTracker::SOIL).units() == Hector::U_PGC, "Doesn't get first element's units correctly"); 

    H_ASSERT(atmos40.getPoolCarbon(CarbonTracker::ATMOSPHERE) == 40, "Doesn't get second element's value correctly");
    H_ASSERT(atmos40.getPoolCarbon(CarbonTracker::ATMOSPHERE).units() == Hector::U_PGC, "Doesn't get second element's units correctly"); 
}

void testFluxToCarbonTracker(){
    Hector::unitval carbon5(5, Hector::U_PGC);
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil10(carbon10, CarbonTracker::SOIL);
    CarbonTracker flux10 = fluxToTracker(carbon10, soil10);
    CarbonTracker flux5 = fluxToTracker(carbon5, soil10);

    cout<< "fluxToTracker Test" << endl;
    H_ASSERT(flux10.getTotalCarbon() == 10, "Flux has the wrong amount of carbon");
    H_ASSERT(sameCTArrays(flux10.getOriginFracs(), soil10.getOriginFracs()), "Flux changes the array from the origin");

    H_ASSERT(flux5.getTotalCarbon() == 5, "Flux has the wrong amount of carbon");
    H_ASSERT(sameCTArrays(flux5.getOriginFracs(), soil10.getOriginFracs()), "Flux changes the array from the origin");
}

void testWrongFluxToTrackerSize(){ 
    Hector::unitval carbon5(5, Hector::U_PGC);
    Hector::unitval carbon10(10, Hector::U_PGC);
    CarbonTracker soil5(carbon5, CarbonTracker::SOIL);
    CarbonTracker flux10 = fluxToTracker(carbon10, soil5);
}

void testWrongFluxToTrackerUnites(){ 
    Hector::unitval carbon5(5, Hector::U_PGC);
    Hector::unitval year10(10, Hector::U_YRS);
    CarbonTracker soil5(carbon5, CarbonTracker::SOIL);
    CarbonTracker flux10 = fluxToTracker(year10, soil5);
}




int main(int argc, char* argv[]){
    cout << "Time for Tests!" << endl;
    testCorrectConstructor();
    //testWrongConstructor();
    testCopyConstructor();
    testAssignmentOperator();
    testAddOperator();
    testSubtractOperator();
    //testWrongSubtraction();
    testSubtractionWithArray();
    testMultiplication();
    testDivision();
    //testDivisionBy0();
    testSetCarbon();
    //testWrongSetCarbon();
    testGetTotalCarbon();
    testGetOriginFracs();
    testGetPoolCarbon();
    testFluxToCarbonTracker();
    //testWrongFluxToTrackerSize();
    //testWrongFluxToTrackerUnites();
    }

