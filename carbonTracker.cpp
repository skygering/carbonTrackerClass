
#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"
#include "unitval.hpp"

using namespace std;

CarbonTracker::CarbonTracker(Hector::unitval totC, Pool subPool){
    H_ASSERT(subPool != CarbonTracker::LAST, "LAST is not a sub-pool of carbon, it is just a marker for the end of the enum")
    H_ASSERT(totC.units() == Hector::U_PGC, "Wrong Units. Carbin tracker only accepts U_PGC");
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

// // PRIVATE - ONLY FOR USE IN FLUX TO CARBON TRACKER FUNCTION
CarbonTracker::CarbonTracker(Hector::unitval totC, double* poolFracs){
    H_ASSERT(totC.units() == Hector::U_PGC, "Wrong Units. Carbin tracker only accepts U_PGC");
    this->totalCarbon = totC;
    double counter = 0;
        for(int i = 0; i< LAST; ++i){
            this->originFracs[i] = *(poolFracs + i);
            counter += *(poolFracs + i);
        }
    H_ASSERT(counter == 1, "Pool fractions don't add up to 1.")
}

// CarbonTracker::~CarbonTracker(){
//     delete[] originFracs;
//     //delete totalCarbon; Does this not work bc unitval doesn't have a constructor? Do I need this?
// }

CarbonTracker::CarbonTracker(const CarbonTracker &ct){
    this->totalCarbon = ct.totalCarbon;
    for(int i = 0; i < CarbonTracker::Pool::LAST; ++i){
        this->originFracs[i] = ct.originFracs[i];
    }
}
CarbonTracker& CarbonTracker::operator=(CarbonTracker ct){
    this->totalCarbon = ct.totalCarbon;
    for(int i = 0; i < CarbonTracker::Pool::LAST; ++i){
        this->originFracs[i] = ct.originFracs[i];
    }
    return *this;
}


CarbonTracker CarbonTracker::operator+(const CarbonTracker& flux){
    Hector::unitval totC = this->totalCarbon + flux.totalCarbon;
    double newOrigins[CarbonTracker::LAST];
    for(int i = 0; i < CarbonTracker::LAST; ++i){
        newOrigins[i] = (this->totalCarbon * this->originFracs[i] + 
                        flux.totalCarbon * flux.originFracs[i]) / totC;
    }
    CarbonTracker addedFlux(totC, newOrigins);
    return addedFlux;
}

CarbonTracker CarbonTracker::operator-(const CarbonTracker& flux){
    Hector::unitval totC = this->totalCarbon - flux.totalCarbon;
    double newOrigins[CarbonTracker::LAST];
    for(int i = 0; i < CarbonTracker::LAST; ++i){
        Hector::unitval poolCarbon = this->totalCarbon * this->originFracs[i] - flux.totalCarbon * flux.originFracs[i];
        H_ASSERT(poolCarbon >= 0, "Pool doesn't have enough carbon to subtract the whole flux - no negative carbon allowed");
        newOrigins[i] = poolCarbon / totC;
    }
    CarbonTracker subtractFlux(totC, newOrigins);
    return subtractFlux;
 }

 CarbonTracker CarbonTracker::operator-(const Hector::unitval flux){
    H_ASSERT(flux.units() == Hector::U_PGC, "Only carbon can be used in carbon tracker!")
    H_ASSERT(this->totalCarbon > flux, "You cannot remove that much carbon, flux is larger than total carbon");
    CarbonTracker ct(this->totalCarbon - flux, this->originFracs);
    return ct;
 }

 CarbonTracker operator*(const double d, CarbonTracker& ct){
    CarbonTracker multipliedCT(ct);
    multipliedCT.setTotalCarbon(multipliedCT.getTotalCarbon() * d);
    return multipliedCT;
 }

 CarbonTracker operator*(const CarbonTracker& ct, const double d){
    CarbonTracker multipliedCT(ct);
    multipliedCT.setTotalCarbon(multipliedCT.getTotalCarbon() * d);
    return multipliedCT;
 }

 CarbonTracker operator/(CarbonTracker& ct, const double d){
    H_ASSERT(d != 0, "No dividing by 0!");
    CarbonTracker multipliedCT(ct);
    multipliedCT.setTotalCarbon(multipliedCT.getTotalCarbon() / d);
    return multipliedCT;
 }

 void CarbonTracker::setTotalCarbon(Hector::unitval tCarbon){
    H_ASSERT(tCarbon.units() == Hector::U_PGC, "Carbon Tracker only accepts unitvals with units U_PGC");
    this->totalCarbon = tCarbon;
 }

//  void CarbonTracker::setOriginFracs(double* poolFracs){
//      for(int i = 0; i< LAST; ++i){
//         this->originFracs[i] = *(poolFracs + i);
//     }
//  }

 Hector::unitval CarbonTracker::getTotalCarbon(){
     return this->totalCarbon;
 }

 double* CarbonTracker::getOriginFracs(){
     return this->originFracs;
 }

Hector::unitval CarbonTracker::getPoolCarbon(Pool subPool){
    H_ASSERT(subPool != CarbonTracker::LAST, "LAST is not a sub-pool of carbon, it is just a marker for the end of the enum");
    return this->originFracs[subPool] * this-> totalCarbon;
}

CarbonTracker fluxToTracker(const Hector::unitval flux, CarbonTracker origin){
    H_ASSERT(flux.units() == Hector::U_PGC, "Flux must be in units U_PGC for carbon tracker");
    H_ASSERT(origin.getTotalCarbon() >= flux, "You don't have enough carbon in the pool to take a flux of that size")
    CarbonTracker ct(origin);
    ct.setTotalCarbon(flux);
    return ct;

}


ostream& operator<<(ostream &out, const CarbonTracker &x ){
    out << "hi";
    return out;
}