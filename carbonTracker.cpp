
#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"
#include "unitval.hpp"

using namespace std;

CarbonTracker::MultiKey::MultiKey(Pool poolName){
    key1 = poolName;
}

bool CarbonTracker::MultiKey::operator<(const MultiKey& right) const{
    return (this->key1 < right.key1);
}


CarbonTracker::CarbonTracker(Hector::unitval totC, MultiKey key){
    totalCarbon = totC;
    unordered_map<CarbonTracker::MultiKey, double> subPool;
    subPool[key] = 1;
    origin_fracs = subPool;
}

CarbonTracker::CarbonTracker(Hector::unitval totC, unordered_map<MultiKey, double> pool_map){

}


CarbonTracker CarbonTracker::operator+(const CarbonTracker& flux){
    return flux;
}

CarbonTracker CarbonTracker::operator-(const CarbonTracker& flux){
    return flux;
 }

 CarbonTracker CarbonTracker::operator-(const Hector::unitval flux){
    CarbonTracker ct(this->totalCarbon, this->origin_fracs);
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

 void CarbonTracker::setCarbonMap(unordered_map<MultiKey, double> org_frac){
     this->origin_fracs = org_frac;
 }

 Hector::unitval CarbonTracker::getTotalCarbon(){
     return this->totalCarbon;
 }

 unordered_map<CarbonTracker::MultiKey, double> CarbonTracker::getCarbonMap(){
     return this->origin_fracs;
 }

Hector::unitval CarbonTracker::getOriginCarbon(MultiKey origin){
    Hector::unitval flux;
    return flux;
}

CarbonTracker fluxToTracker(const Hector::unitval flux, unordered_map<CarbonTracker::MultiKey, double> origin_fracs){
    CarbonTracker ct(flux, origin_fracs);
    return ct;

}

void CarbonTracker::startTracking(){
    track = true;
}


ostream& operator<<(ostream &out, const CarbonTracker &x ){
    out << "hi";
    return out;
}