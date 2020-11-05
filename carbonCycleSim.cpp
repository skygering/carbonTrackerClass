#include "carbonCycleSim.hpp"
using namespace std;

Hector::unitval carbon20(20, Hector::U_PGC);
Hector::unitval carbon10(10, Hector::U_PGC);
Hector::unitval carbon100(100, Hector::U_PGC);

void carbonCycleSimBasic(){

    CarbonTracker soil(carbon100, CarbonTracker::SOIL);
    CarbonTracker atmos(carbon100, CarbonTracker::ATMOSPHERE);
    CarbonTracker deepOcean(carbon100, CarbonTracker::DEEPOCEAN);
    CarbonTracker topOcean(carbon100, CarbonTracker::TOPOCEAN);

    CarbonTracker::startTracking();

    CarbonTracker soil10Flux = soil.fluxFromTrackerPool(carbon10); // example of creating a flux
    soil = soil - soil10Flux; // remove the flux from soil pool
    atmos = atmos + soil10Flux; // add flux to atmosphere pool

    CarbonTracker halfDeepOceanFlux = 0.5* deepOcean; // you can also create fluxes by multiplying 
    deepOcean = deepOcean - halfDeepOceanFlux;
    topOcean = topOcean + halfDeepOceanFlux;

    CarbonTracker quarterAtmosFlux = atmos/4;
    soil = soil + quarterAtmosFlux;
    atmos = atmos - quarterAtmosFlux;

    soil =  soil - carbon10; // can also remove flux with subtraction of just carbon
    deepOcean = deepOcean + soil10Flux; // however you still need to make a flux to add to another pool (using premade flux for simplicity)

    cout<<"Soil: \n" << soil<<endl; // you can print a tracker object and it gives total carbon and amount in every pool
    cout<<"Atmosphere: \n"<< atmos<<endl; // let me know if there is different info that you want
    cout<<"Deep Ocean: \n"<< deepOcean<<endl;
    cout<<"Top Ocean: \n" << topOcean<<endl;

    cout<<"We we tracking? "<< CarbonTracker::isTracking()<<endl; //can check if you're tracking

    soil.setTotalCarbon(carbon10); // you can set the total carbon although I am not sure you should be able to - thoughts? 

    Hector::unitval totSoilCarbon = soil.getTotalCarbon(); // able to get total carbon
    Hector::unitval atmosCFromSoil = soil.getPoolCarbon(CarbonTracker::Pool::ATMOSPHERE);
    double* soilFracs = soil.getOriginFracs();  // able to get the fractions from a pool
    cout<< "Soil total carbon: "<< totSoilCarbon <<endl; // able to get carbon from just one pool
    cout<< "Soil carbon from atmosphere: " << atmosCFromSoil <<endl;
    for(int i = 0; i<CarbonTracker::Pool::LAST; ++i){
        cout<<"Soil index " <<CarbonTracker::POOLNAMES[i]<<": "<< soilFracs[i]<<endl;
    }


    CarbonTracker::stopTracking();

}

void carbonCycleSimLoops(){

    CarbonTracker soil(carbon100, CarbonTracker::SOIL);
    CarbonTracker atmos(carbon100, CarbonTracker::ATMOSPHERE);
    CarbonTracker deepOcean(carbon100, CarbonTracker::DEEPOCEAN);
    CarbonTracker topOcean(carbon100, CarbonTracker::TOPOCEAN);

    CarbonTracker allPools[] = {soil, atmos, deepOcean, topOcean};

       // for(int i = 1; i < CarbonTracker::LAST; ++i){
    //     soil = soil - soil10Flux;
    //     allPools[i] = allPools[i] + soil10Flux;
    // }
    // cout<<"Soil: \n" << soil<<endl;
    // cout<<"Atmosphere: \n"<< atmos<<endl;
    // cout<<"Deep Ocean: \n"<< deepOcean<<endl;
    // cout<<"Top Ocean: \n" << topOcean<<endl;
}

void carbonCycleSimFun(){
    // TODO: Have fun! Let me know if you have any questions! 
}