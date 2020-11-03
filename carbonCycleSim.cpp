// void carbonCycleSim(){
//     Hector::unitval carbon20(20, Hector::U_PGC);
//     Hector::unitval carbon10(10, Hector::U_PGC);
//     Hector::unitval carbon100(100, Hector::U_PGC);

//     CarbonTracker soil(carbon100, CarbonTracker::SOIL);
//     CarbonTracker atmos(carbon100, CarbonTracker::ATMOSPHERE);
//     CarbonTracker deepOcean(carbon100, CarbonTracker::DEEPOCEAN);
//     CarbonTracker topOcean(carbon100, CarbonTracker::TOPOCEAN);

//     CarbonTracker allPools[] = {soil, atmos, deepOcean, topOcean};

//     soil10Flux = soil.fluxFromTrackerPool(carbon10);
//     for(int i = 1; i < CarbonTracker::LAST; ++i){
//         soil = soil - soilFlux10;
//         allPools[i] = allPools[i] + soilFlux10;
//     }
//     cout<<soil<<endl;


// }
