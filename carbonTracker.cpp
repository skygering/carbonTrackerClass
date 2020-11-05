
#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"
#include "unitval.hpp"

using namespace std;
bool CarbonTracker::track = false;
string POOLNAMES[] = {"Soil", "Atmosphere", "Deep Ocean", "Top Ocean"};

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

// PRIVATE - ONLY FOR USE IN FLUX TO CARBON TRACKER FUNCTION
CarbonTracker::CarbonTracker(Hector::unitval totC, double* poolFracs){
    H_ASSERT(totC.units() == Hector::U_PGC, "Wrong Units. Carbin tracker only accepts U_PGC");

    this->totalCarbon = totC;
    double counter = 0;
    for(int i = 0; i< LAST; ++i){
        double frac = poolFracs[i];
        this->originFracs[i] = frac;
        //H_ASSERT(frac>=0, "Can't have negative proportion of a carbon pool");
        counter += frac;
    }
}


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
    if(!track){
        double fluxAddedPoolCheck = 0; 
        for(int i = 0; i < CarbonTracker::LAST; ++i){
            newOrigins[i] = (totC * this->originFracs[i] + 
                        totC * flux.originFracs[i]) / totC;
            fluxAddedPoolCheck += (this->originFracs[i] + flux.originFracs[i]);
        }
        // fluxToTrackerPool makes the originFracs of a pool all 0 if it is not tracking so that it won't mess up
        // the fractions of the pool the flux is added to - since tracking is off pool should only have 1 non-zero
        // array element from the public constructor
        H_ASSERT(fluxAddedPoolCheck == 1, "You can only add a flux to a pool, not a pool to a pool!")
    }
    else{
        for(int i = 0; i < CarbonTracker::LAST; ++i){
            newOrigins[i] = (this->totalCarbon * this->originFracs[i] + 
                        flux.totalCarbon * flux.originFracs[i]) / totC;
        }
    }
    CarbonTracker addedFlux(totC, newOrigins);
    return addedFlux;
}

// USEFUL FOR WHEN YOU WANT TO REMOVE CARBON UNEQUALLY FROM DIFFERENT POOLS
// This will be usful for isotopes but not for general use
// Order matters - flux object's carbon removed from pool object's carbon
CarbonTracker CarbonTracker::operator-(const CarbonTracker& flux){
    Hector::unitval totC = this->totalCarbon - flux.totalCarbon;
    if(!CarbonTracker::track){
        return *this - flux.totalCarbon; // calls below operator- method that takes unitvals
    }
    else{
        double newOrigins[CarbonTracker::LAST];
        for(int i = 0; i < CarbonTracker::LAST; ++i){
            Hector::unitval poolCarbon = this->totalCarbon * this->originFracs[i] - flux.totalCarbon * flux.originFracs[i];
            //H_ASSERT(poolCarbon >= 0, "Pool doesn't have enough carbon to subtract the whole flux - no negative carbon allowed");
            newOrigins[i] = poolCarbon / totC;
        }
        CarbonTracker subtractFlux(totC, newOrigins);
        return subtractFlux;
    }
    
 }

// Removes carbon from each of the subpools equally by proportion
// Order matters - will keep 'pools' array when a flux is subtracted from a pool
 CarbonTracker CarbonTracker::operator-(const Hector::unitval flux){
    H_ASSERT(flux.units() == Hector::U_PGC, "Only carbon can be used in carbon tracker!")
    //H_ASSERT(this->totalCarbon > flux, "You cannot remove that much carbon, flux is larger than total carbon");
    CarbonTracker ct(this->totalCarbon - flux, this->originFracs);
    return ct;
 }

// order matters - see below operator* for other order
 CarbonTracker operator*(const double d, CarbonTracker& ct){
    CarbonTracker multipliedCT(ct);
    multipliedCT.setTotalCarbon(multipliedCT.getTotalCarbon() * d);
    if(!CarbonTracker::track){
        for(int i = 0; i<CarbonTracker::Pool::LAST; ++i){ 
            multipliedCT.originFracs[i] = 0; // if we are not tracking we want flux arrays to be all 0s
        }
    }
    return multipliedCT;
 }

 CarbonTracker operator*(const CarbonTracker& ct, const double d){
    CarbonTracker multipliedCT(ct);
    multipliedCT.setTotalCarbon(multipliedCT.getTotalCarbon() * d);
    if(!CarbonTracker::track){
        for(int i = 0; i<CarbonTracker::Pool::LAST; ++i){
            multipliedCT.originFracs[i] = 0; // if we are not tracking we want flux arrays to be all 0s
        }
    }
    return multipliedCT;
 }

 CarbonTracker operator/(CarbonTracker& ct, const double d){
    H_ASSERT(d != 0, "No dividing by 0!");
    CarbonTracker dividedCT(ct);
    dividedCT.setTotalCarbon(dividedCT.getTotalCarbon() / d);
    if(!CarbonTracker::track){
        for(int i = 0; i<CarbonTracker::Pool::LAST; ++i){
            dividedCT.originFracs[i] = 0; // if we are not tracking we want flux arrays to be all 0s
        }
    }
    return dividedCT;
 }

 void CarbonTracker::setTotalCarbon(Hector::unitval tCarbon){
    H_ASSERT(tCarbon.units() == Hector::U_PGC, "Carbon Tracker only accepts unitvals with units U_PGC");
    //H_ASSERT(tCarbon >=0, "Cannot set total carbon to a negative number!");
    this->totalCarbon = tCarbon;
 }

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

bool CarbonTracker::isTracking(){
      return CarbonTracker::track;
}

void CarbonTracker::startTracking(){
        CarbonTracker::track = true;
}

void CarbonTracker::stopTracking(){
        CarbonTracker::track = false;
}

CarbonTracker CarbonTracker::fluxFromTrackerPool(const Hector::unitval flux){
    H_ASSERT(flux.units() == Hector::U_PGC, "Flux must be in units U_PGC for carbon tracker");
    //H_ASSERT(this->totalCarbon >= flux, "You don't have enough carbon in the pool to make a flux of that size");
    CarbonTracker ct(*this);
    ct.setTotalCarbon(flux);
    if(!track){
        for(int i = 0; i<CarbonTracker::LAST; ++i){
            ct.originFracs[i] = 0;
            // if not tracking then the array is all 0s because the flux should not change original arrays
        }
    }
    return ct;
}


// USEFUL FOR WHEN YOU WANT TO MAKE A CUSTOM FLUX WITH DIFFERENT POOLS OF CARBON BEING PULLED FROM MORE THAN OTHERS
// This will be usful for isotopes but not for general use
CarbonTracker CarbonTracker::fluxFromTrackerPool(const Hector::unitval fluxAmount, double* fluxProportions){
    H_ASSERT(fluxAmount.units() == Hector::U_PGC, "Flux must be in units U_PGC for carbon tracker");
    //H_ASSERT(this->totalCarbon >= fluxAmount, "You don't have enough carbon in the pool to make a flux of that size");
    double fluxFracs[CarbonTracker::LAST];
    if(!track){
        for(int i = 0; i<CarbonTracker::LAST; ++i){
            fluxFracs[i] = 0;
            // if not tracking then the array is all 0s because the flux should not change original arrays
        }
    }
    else{ 
        double fracTotal = 0;
        for(int i = 0; i<CarbonTracker::LAST; ++i){
            fluxFracs[i] = fluxProportions[i];
            fracTotal = fracTotal + fluxFracs[i];
        }
        H_ASSERT(fracTotal == 1, "You must enter in proporations of carbon that add to 1");
    }
    return CarbonTracker(fluxAmount, fluxFracs);
}

ostream& operator<<(ostream &out, CarbonTracker &ct ){
    cout<<"Total Carbon: "<< ct.getTotalCarbon()<<endl;
    for(int i = 0; i<CarbonTracker::LAST; ++i){
       cout << POOLNAMES[i]<<": "<< ct.getPoolCarbon((CarbonTracker::Pool)i)<<endl;
    }
    return out;
}