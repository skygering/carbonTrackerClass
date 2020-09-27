
#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"

using namespace std;

CarbonTracker::MultiKey::MutliKey(Pool poolName){
    key1 = poolName;
}

bool MultiKey::operator<(const MutliKey& right) const{
    return (this.key1 < right.key1);
}

// CarbonTracker::~CarbonTracker(){

// }
// CarbonTracker::CarbonTracker (const CarbonTracker& ct);

CarbonTracker::CarbonTracker(Hector::unitval totalCarbon, MultiKey key)
              :totalCarbon_{totalCarbon}, track_{track}{
                unordered_map<CarbonTracker::MultiKey, double> carbonMap({{key,1}});
                origin_fracs = carbonMap;
              }

CarbonTracker::CarbonTracker(Hector::unitval totalCarbon, unordered_map<MultiKey, double> origin_fracs)
              :totalCarbon_{totalCarbon},  origin_fracs_{origin_frac}, track_{track}{}


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

CarbonTracker CarbonTracker::fluxToTracker(const Hector::unitval flux, unordered_map<MultiKey, double> origin_fracs){
    return CarbonTracker::CarbonTracker(flux, origin_fracs);
}

static void CarbonTracker::startTracking(){
    track = true;
}


ostream& operator<<(ostream &out, const CarbonTracker &x ){
    out << "hi";
    return out;
}