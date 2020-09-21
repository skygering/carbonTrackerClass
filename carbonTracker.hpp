#ifndef CARBONTRACKER_HPP
#define CARBONTRACKER_HPP

/*
carbontracker.hpp - a class to track the origin of carbon within various carbon pool 
*/

#include <sstream>
#include <unordered_map> 
#include "unitval.hpp"
using namespace std; 

class carbonTracker{
  private:
    unordered_map<string, double>  origin_val; 
    double totalCarbon;
    bool track;

  public:
    carbonTracker();
    carbonTracker(unordered_map<string, double> origin_val, double totalCarbon, bool track = true);


    friend carbonTracker operator+(const carbonTracker& flux);
    friend carbonTracker operator+(const Hector::unitval flux);

    
    friend carbonTracker operator-(const carbonTracker& flux);
    friend carbonTracker operator-(const Hector::unitval d);

    friend carbonTracker operator*(const double d, carbonTracker& ct);
    friend carbonTracker operator*(const carbonTracker& ct, double d);

    friend carbonTracker operator/(const double, carbonTracker&);
    // I am not sure this means anything... thus maybe we don't want (see above comment on multiplication)
    friend carbonTracker operator/(carbonTracker&, const double);


    void set(unordered_map<string, double> origin_vals, double totalCarbon);
    void setTotalCarbon(Hector::unitval totalCarbong);

    // gotta work on using set as a Key... is this possible
    void setOriginPercent(unordered_map<string, double> origin_vals);

    Hector::unitval getTotalCarbon();
    unordered_map<string, double> getOriginPercent();
    Hector::unitval getOriginCarbon(string origin);

    carbonTracker fluxToTracker(Hector::unitval flux);
};
std::ostream& operator<<( std::ostream &out, const carbonTracker &x );

#endif