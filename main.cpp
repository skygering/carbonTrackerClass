#include "carbonTracker.hpp"
using namespace std;

bool testConstructor(){
    Hector::unitval carbon(10, Hector::U_PGC);
    CarbonTracker soil(carbon, CarbonTracker::SOIL);
    double* arr = soil.getOriginFracs();
    bool correctArray = true;
    for(int i = 0; i< CarbonTracker::LAST; ++i){
        if(arr[i] != 0 && i != CarbonTracker::SOIL){
            correctArray = false;
        }
        if(i==CarbonTracker::SOIL && arr[i] != 1){
            correctArray = false;
        }
    }
    return (soil.getTotalCarbon() == 10) && correctArray;
}


int main(int argc, char* argv[]){
    cout << "Time for Tests!" << endl;
    cout << testConstructor() << endl;
}

