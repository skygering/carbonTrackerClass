#ifndef CARBONTRACKER_HPP
#define CARBONTRACKER_HPP

/*
carbontracker.hpp - a class to track the origin of carbon within various carbon pool 
*/

#include <sstream>
#include <unordered_map> 
using namespace std; 

class carbonTracker{
  private:
    unordered_map<string, double>  origin_val; 
    double totalCarbon;

  public:
    carbonTracker();
    carbonTracker(unordered_map<string, double> origin_val, double totalCarbon);

    // do we want to add double or unitvals? Right now the carbon has no units...
    // how do we know where we are adding from? We aren't really adding two carbon trackers,
    // we are adding the change in one carbon tracker from a flux to another carbon tracker
    // the way I have written declaration it will modfy c1 and then return C1 --> okay?
    friend carbonTracker operator+(carbonTracker& ct1, const carbonTracker& ct2);
    friend carbonTracker operator+(carbonTracker& ct,  const double d);
    friend carbonTracker operator+(double d, const carbonTracker& ct); 
    // do we ever want ct +d = d or should people use get value for that?

    // kinda want to modify c1 and then spit out the flux... but that seems counterintuitive
    // could also make a function that makes a flux into a carbon tracker based off of where 
    // it is suubtracted from. That seems like the most intuitive way --> can use getTotalCarbon
    // and get map and then use the set function for flux
    friend carbonTracker operator-(carbonTracker& ct1,  const carbonTracker& ct2);
    friend carbonTracker operator-(carbonTracker& ct, const double d);
    friend carbonTracker operator-(double d, const carbonTracker& ct);
    friend carbonTracker operator-(carbonTracker& ct);

    friend carbonTracker operator*(const double d, carbonTracker& ct);
    friend carbonTracker operator*(carbonTracker& ct, const double d);
    // do we need tracker*tracker ? --> this doesn't seems to have much utility

    friend carbonTracker operator/(carbonTracker&, const carbonTracker&);
    // I am not sure this means anything... thus maybe we don't want (see above comment on multiplication)
    friend carbonTracker operator/(carbonTracker&, const double);

    friend std::ostream& operator<<( std::ostream &out, const carbonTracker &x );

    void set(unordered_map<string, double> origin_vals, double totalCarbon);
    void setTotalCarbon(double totalCarbong);
    void setOriginPercent(unordered_map<string, double> origin_vals);
    double getTotalCarbon();
    unordered_map<string, double> getOriginPercent();

    void fluxToTracker(carbonTracker ct, double d);
};