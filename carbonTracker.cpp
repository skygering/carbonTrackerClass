
#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"

using namespace std;

MultiKey::MutliKey(string key1){
    poolName = key1;
}

bool MultiKey::operator<(const MutliKey& right) const{
    return (this.poolName < right.poolName);
}

~CarbonTracker();
CarbonTracker::CarbonTracker (const CarbonTracker& ct);

CarbonTracker::CarbonTracker(Hector::unitval totalCarbon, MultiKey key, bool track = false)
              :totalCarbon_{totalCarbon}, track_{track}{
                unordered_map<CarbonTracker::MultiKey, double> carbonMap({{key,1}});
                origin_fracs = carbonMap;
              }

CarbonTracker::CarbonTracker(unordered_map<MultiKey, double> origin_val, Hector::unitval totalCarbon, bool track = true)
              :origin_Cal_{origin_val}, totalCarbon_{totalCarbon}, track_{track}{}

CarbonTracker CarbonTracker::operator+(const CarbonTracker& flux){
    return this;
}

CarbonTracker CarbonTracker::operator-(const CarbonTracker& flux){
     return this;
 }

 CarbonTracker CarbonTracker::operator-(const Hector::unitval flux){
     return this;
 }

 CarbonTracker CarbonTracker::operator*(const double d, CarbonTracker& ct){
     return ct;
 }

 CarbonTracker CarbonTracker::operator*(const CarbonTracker& ct, double d){
     return this;
 }

 CarbonTracker CarbonTracker::operator/(CarbonTracker&, const double){
     return this;
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

 unordered_map<MultiKey, double> CarbonTracker::getCarbonMap(){
     return this->origin_fracs;
 }

Hector::unitval CarbonTracker::getOriginCarbon(MultiKey origin){
    return this->origin_fracs.find(origin);
}

CarbonTracker CarbonTracker::fluxToTracker(const Hector::unitval flux, MultiKey key){
    return CarbonTracker::CarbonTracker(flux, key);
}

void CarbonTracker::startTracking(){
    this->track = true;
}


ostream& operator<<(ostream &out, const CarbonTracker &x ){
    out << "hi";
    return out;
}