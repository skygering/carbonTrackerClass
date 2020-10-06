
#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"
#include "unitval.hpp"

using namespace std;

CarbonTracker::CarbonTracker(Hector::unitval totC, Pool subPool){
    this->totalCarbon = totC;
    for(int i = 0; i< LAST; ++i){
        if(i == subPool){
            this->originFracs[i] = 1;
        }
        else{
            this->originFracs[i] = 0;
        }
    }
}

CarbonTracker::CarbonTracker(Hector::unitval totC, double* poolFracs){
    this->totalCarbon = totC;
    for(int i = 0; i< LAST; ++i){
        this->originFracs[i] = *(poolFracs + i);
    }
}


CarbonTracker CarbonTracker::operator+(const CarbonTracker& flux){
    return flux;
}

CarbonTracker CarbonTracker::operator-(const CarbonTracker& flux){
    return flux;
 }

 CarbonTracker CarbonTracker::operator-(const Hector::unitval flux){
    CarbonTracker ct(this->totalCarbon, this->originFracs);
    return ct;
 }

 CarbonTracker operator*(const double d, CarbonTracker& ct){
     return ct;
 }

 CarbonTracker operator*(const CarbonTracker& ct, double d){
     return ct;
 }

 CarbonTracker operator/(CarbonTracker& ct, const double){
     return ct;
 }

 void CarbonTracker::setTotalCarbon(Hector::unitval tCarbon){
     this->totalCarbon = tCarbon;
 }

 void CarbonTracker::setOriginFracs(double* poolFracs){
     for(int i = 0; i< LAST; ++i){
        this->originFracs[i] = *(poolFracs + i);
    }
 }

 Hector::unitval CarbonTracker::getTotalCarbon(){
     return this->totalCarbon;
 }

 double* CarbonTracker::getOriginFracs(){
     return this->originFracs;
 }

Hector::unitval CarbonTracker::getOriginCarbon(Pool subPool){
    return this->originFracs[subPool] * this-> totalCarbon;
}

CarbonTracker fluxToTracker(const Hector::unitval flux, double* origin_fracs){
    CarbonTracker ct(flux, origin_fracs);
    return ct;

}

//void CarbonTracker::startTracking(){
//  track = true;
//}


ostream& operator<<(ostream &out, const CarbonTracker &x ){
    out << "hi";
    return out;
}