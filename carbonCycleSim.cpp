#include "carbonCycleSim.hpp"
using namespace std;

Hector::unitval carbon20(20, Hector::U_PGC);
Hector::unitval carbon10(10, Hector::U_PGC);
Hector::unitval carbon200(200, Hector::U_PGC);

void carbonCycleSimBasic(){
    cout<<"Basic Simulation:"<<endl;

    CarbonTracker soil(carbon200, CarbonTracker::SOIL);
    CarbonTracker atmos(carbon200, CarbonTracker::ATMOSPHERE);
    CarbonTracker deepOcean(carbon200, CarbonTracker::DEEPOCEAN);
    CarbonTracker topOcean(carbon200, CarbonTracker::TOPOCEAN);

    CarbonTracker::startTracking();

    CarbonTracker soil10Flux = soil.fluxFromTrackerPool(carbon10); // example of creating a flux
    soil = soil - soil10Flux; // remove the flux from soil pool
    atmos = atmos + soil10Flux; // add flux to atmosphere pool

    CarbonTracker halfDeepOceanFlux = 0.5* deepOcean; // you can also create fluxes by multiplying 
    deepOcean = deepOcean - halfDeepOceanFlux;
    topOcean = topOcean + halfDeepOceanFlux;

    CarbonTracker quarterAtmosFlux = atmos/4; // and also by division!
    soil = soil + quarterAtmosFlux;
    atmos = atmos - quarterAtmosFlux;

    soil =  soil - carbon10; // can also remove flux with subtraction of just carbon
    soil10Flux = soil.fluxFromTrackerPool(carbon10);
    deepOcean = deepOcean + soil10Flux; // however you still need to make a flux to add to another pool 

    double fluxArr[] = {0,0, 0.5, 0.5};
    CarbonTracker topOceanFluxFromOcean = topOcean.fluxFromTrackerPool(carbon10, fluxArr); // takes carbon in specific ratios - this is what you will want to use for isotopes
    topOcean = topOcean - topOceanFluxFromOcean;
    deepOcean = deepOcean + topOceanFluxFromOcean;

    cout<<"Soil: \n" << soil<<endl; // you can print a tracker object and it gives total carbon and amount in every pool
    cout<<"Atmosphere: \n"<< atmos<<endl; // let me know if there is different info that you want
    cout<<"Deep Ocean: \n"<< deepOcean<<endl;
    cout<<"Top Ocean: \n" << topOcean<<endl;

    cout<<"Are we tracking? "<< CarbonTracker::isTracking()<<endl; //can check if you're tracking

    soil.setTotalCarbon(carbon10); // you can set the total carbon although I am not sure you should be able to - thoughts? 

    Hector::unitval totSoilCarbon = soil.getTotalCarbon(); // able to get total carbon
    Hector::unitval atmosCFromSoil = soil.getPoolCarbon(CarbonTracker::Pool::ATMOSPHERE);
    double* soilFracs = soil.getOriginFracs();  // able to get the fractions from a pool
    cout<< "Soil total carbon: "<< totSoilCarbon <<endl; // able to get carbon from just one pool
    cout<< "Soil carbon from atmosphere: " << atmosCFromSoil <<endl;
    for(int i = 0; i<CarbonTracker::Pool::LAST; ++i){
        cout<<"Soil index "<< i <<": "<< soilFracs[i]<<endl;
        //I cant get POOLNAMES[i] to compile... I know its due to static stuff but I don't know why. I don't know if this is needed
    }

    CarbonTracker::stopTracking();
}

void carbonCycleSimLoops(){
    cout<<"Loop Simulation:"<<endl;

    CarbonTracker soil(carbon200, CarbonTracker::SOIL);
    CarbonTracker atmos(carbon200, CarbonTracker::ATMOSPHERE);
    CarbonTracker deepOcean(carbon200, CarbonTracker::DEEPOCEAN);
    CarbonTracker topOcean(carbon200, CarbonTracker::TOPOCEAN);

    CarbonTracker::startTracking();


    for(int i = 1; i < 5; ++i){
        CarbonTracker soilFlux20 = soil.fluxFromTrackerPool(carbon20);
        soil = soil - soilFlux20;
        atmos = atmos + soilFlux20;

        CarbonTracker atmosFlux20 = atmos.fluxFromTrackerPool(carbon20);
        atmos = atmos - atmosFlux20;
        topOcean = topOcean + atmosFlux20;

        CarbonTracker topOceanFlux20 = topOcean.fluxFromTrackerPool(carbon20);
        topOcean = topOcean - topOceanFlux20;
        deepOcean = deepOcean + topOceanFlux20;
        
        CarbonTracker deepOceanFlux20 = deepOcean.fluxFromTrackerPool(carbon20);
        deepOcean = deepOcean - deepOceanFlux20;
        soil = soil + deepOceanFlux20;

    }
    cout<<"Soil: \n" << soil<<endl;
    cout<<"Atmosphere: \n"<< atmos<<endl;
    cout<<"Deep Ocean: \n"<< deepOcean<<endl;
    cout<<"Top Ocean: \n" << topOcean<<endl;
    CarbonTracker::stopTracking();
}

void carbonCycleSimFun(){
    cout<<"Fun Simulation:"<<endl;
    // TODO: Have fun! Let me know if you have any questions! 
}