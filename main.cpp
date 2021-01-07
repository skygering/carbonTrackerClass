#include <iostream>     
#include <cassert> 
#include "TrackedVal.hpp"
#include <chrono>

using namespace std;

#define ASSERT_NOTHROW(zzz,msg){\
bool thrown=false; \
try{zzz;} \
catch (h_exception e) {thrown=true;} \
H_ASSERT(thrown, msg); \
}

void test_creation() {
    cout << "test_creation" << endl;
    TrackedVal x(Hector::unitval(), "x");
    H_ASSERT(!x.isTracking(), "Tracker doesn't start off false");
    H_ASSERT(x.get_total() == Hector::unitval(), "Total isn't unitval");
    x.setTracking(true);
    H_ASSERT(x.get_fraction("x") == 1.0, "x fraction isn't 1");
}

void test_tracking(){
    cout << "test_tracking" << endl;
    TrackedVal x(Hector::unitval(), "x");
    x.setTracking(true);
    H_ASSERT(x.isTracking(), "Turning on tracking doesn't work");
    x.setTracking(false);
    H_ASSERT(!x.isTracking(), "Turning off tracking doesn't work");
}

void test_get_sources(){
    cout << "test_get_sources" << endl;
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    ASSERT_NOTHROW(x.get_sources(), "get_sources didn't throw exception");
        
    x.setTracking(true);
    vector<string> x_sources = x.get_sources();
    H_ASSERT(x_sources.size() == 1, "wrong size");
    H_ASSERT(x_sources[0] == "x", "wrong source");
    
    TrackedVal y(c1, "y");
    y.setTracking(true);
    TrackedVal z = x + y;
    vector<string> z_sources = z.get_sources();
    H_ASSERT(z_sources.size() == 2, "wrong size");
    H_ASSERT(z_sources[0] == "x", "wrong source");
    H_ASSERT(z_sources[1] == "y", "wrong source");
}

void test_get_total(){
    cout << "test_get_total" << endl;
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    Hector::unitval total = x.get_total();
    H_ASSERT(total == c1, "Total doesn't match");
}

void test_get_fraction(){
    cout << "test_get_fraction" << endl;
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    ASSERT_NOTHROW(x.get_fraction("x"), "get_fraction didn't throw exception")
    x.setTracking(true);
    double frac = x.get_fraction("x");
    H_ASSERT(frac == 1, "initial frac not 1");
}

void test_addition(){
    cout << "test_addition" << endl;
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    TrackedVal y(c1, "y");
    
    // addtion without tracking
    TrackedVal z = x + y;
    Hector::unitval c2 = c1 + c1;
    H_ASSERT(z.get_total() == c2, "untracked total not right");
    
    // tracking mismatch
    x.setTracking(true);
    ASSERT_NOTHROW(x + y, "addition didn't throw mismatch exception")
    x.setTracking(false);
    y.setTracking(true);
    ASSERT_NOTHROW(x + y, "addition didn't throw mismatch exception")

    // addition with tracking
    x.setTracking(true);
    y.setTracking(true);
    z = x + y;
    H_ASSERT(z.get_total() == c2, "tracked total not right");
    H_ASSERT(z.get_fraction("x") == 0.5, "x fraction not right");
    H_ASSERT(z.get_fraction("y") == 0.5, "x fraction not right");
    
    z = z + x;
    H_ASSERT(z.get_total() == c1 + c2, "tracked total not right");
    double xfrac = z.get_fraction("x");
    double yfrac = z.get_fraction("y");
    H_ASSERT(xfrac == yfrac * 2, "x/y fractions not right");
    
    // add 0
    Hector::unitval c0(0, Hector::U_PGC);
    TrackedVal zero(c0, "zero");
    zero.setTracking(true);
    TrackedVal x2 = x + zero;
    H_ASSERT(x == x2, "adding zero doesn't work");
    H_ASSERT(x2.get_fraction("x") == 1, "x2 fraction not right");
    
    // add 0 and 0
    TrackedVal zerozero = zero + zero;
    H_ASSERT(zerozero == zero, "adding zeros doesn't work");

    // add after turning tracking off - I don't think that this scenario really matters
    // y.setTracking(false);
    // x.setTracking(false);
    // cout<<y<<endl;
    // y = y + x; // now has a value of c2
    // y.setTracking(true);
    // x.setTracking(true);
    // cout<<"y: " << y << "y pools:" << y.get_fraction("y") <<endl;
    // H_ASSERT(y.get_total() == c2, "adding after turning tracking off doesn't work");
    // H_ASSERT(y.get_fraction("y") == 1, "z total not right (tracked off/on)");
    // H_ASSERT(y.get_fraction("x") == 0, "x total not right (tracked off/on)");

    // temp addition without tracking
    x.setTracking(false);
    Hector::unitval c3(3, Hector::U_PGC);
    TrackedVal w = x + c3;
    H_ASSERT(w.get_total() == c1 + c3, "temp untracked total not right");

    //temp addition with tracking
    x.setTracking(true);
    w = x + c3;
    H_ASSERT(w.get_total() == c1 + c3, "temp tracked total not right");
    H_ASSERT(w.get_fraction("x") == 0.25, "w fraction not right");
    H_ASSERT(w.get_fraction("not tracked") == 0.75, "w fraction not right");

    //temp addition add 0
    TrackedVal w2 = w + c0;
    H_ASSERT(w2 == w, "temp adding zero doesn't work");
    H_ASSERT(w2.get_fraction("not tracked") == 0.75, "w2 fraction not right");
    H_ASSERT(w2.get_fraction("x") == 0.25, "w2 fraction not right");

}

void test_subtraction(){
    cout << "test_subtraction" << endl;
    Hector::unitval c0(0, Hector::U_PGC);
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    x.setTracking(true);
    
    // can also subtract a unitval
    TrackedVal y = x - c1;
    H_ASSERT(y.get_total() == 0, "unitval subtraction doesn't work");
    TrackedVal x2 = x - c0;
    H_ASSERT(x.get_total() == x2.get_total(), "unitval 0 subtraction doesn't work");
}

void test_mult_div(){
    cout << "test_multiplication and division" << endl;
    Hector::unitval c0(0, Hector::U_PGC);
    Hector::unitval c1(1, Hector::U_PGC);
    Hector::unitval c2(2, Hector::U_PGC);
    TrackedVal x(c2, "x");
    x.setTracking(true);
    
    TrackedVal x2 = x * 2;
    H_ASSERT(x2.get_total() == x.get_total() * 2.0, "member multiplication doesn't work");
    H_ASSERT(x.get_fraction("x") == x2.get_fraction("x"), "mult fractions not preserved");
    TrackedVal x2a = 2 * x;
    H_ASSERT(x2.get_total() == x2a.get_total(), "nonmember mult doesn't work");
    x2 = x / 2.0;
    H_ASSERT(x2.get_total() == x.get_total() / 2.0, "division doesn't work");
    H_ASSERT(x.get_fraction("x") == x2.get_fraction("x"), "div fractions not preserved");
    TrackedVal x5a = x * 0.2;
    TrackedVal x5b = x / 5.0;
    H_ASSERT(x5a.get_total() == x5b.get_total(), "mult or division doesn't work");
    TrackedVal x1 = x * 1;
    H_ASSERT(x1.get_total() == x.get_total(), "multiplication identity doesn't work");
    x1 = x / 1;
    H_ASSERT(x1.get_total() == x.get_total(), "division identity doesn't work");
    TrackedVal x0 = x * 0;
    H_ASSERT(x0.get_total() == c0, "multiplication by zero doesn't work");
    x0 = x / 0;
    H_ASSERT(isinf(x0.get_total()), "division by zero doesn't work");
    
}

void test_equality(){
    cout << "test_equality" << endl;
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    TrackedVal y(c1, "x");
    
    H_ASSERT(x == y, "equality doesn't work");
    H_ASSERT(!(x != y), "inequality doesn't work");
    y = x * 2;
    H_ASSERT(x != y, "equality doesn't work");
    H_ASSERT(!(x == y), "inequality doesn't work");
}

void test_identicality(){
    cout << "test_identicality" << endl;
    Hector::unitval c1(1, Hector::U_PGC);
    TrackedVal x(c1, "x");
    TrackedVal x1(c1, "x");
    TrackedVal y(c1, "y");
    
    H_ASSERT(x.identical(x1), "identical doesn't work");
    
    x.setTracking(true);
    H_ASSERT(!x.identical(x1), "identical tracking doesn't work");
    H_ASSERT(!x1.identical(x), "identical tracking doesn't work");
    x1.setTracking(true);
    H_ASSERT(x.identical(x1), "identical tracking doesn't work");
    x.setTracking(false);
    H_ASSERT(!x.identical(x1), "identical tracking doesn't work");
    H_ASSERT(!x1.identical(x), "identical tracking doesn't work");
    x.setTracking(true);
    
    // different sources
    y.setTracking(true);
    H_ASSERT(!x.identical(y), "identical sources doesn't work");
    H_ASSERT(!y.identical(x), "identical sources doesn't work");
    
    // different fraction
    Hector::unitval c0(0, Hector::U_PGC);
    TrackedVal x0(c0, "x");
    x0.setTracking(true);
    H_ASSERT(!x.identical(x0), "identical fractions doesn't work");
    H_ASSERT(!x0.identical(x), "identical fractions doesn't work");
}

int main(int argc, char* argv[]){
    
    Hector::unitval carbon10(10, Hector::U_PGC);
    TrackedVal x(carbon10, "x");
    cout << "x is " << x << endl;
    x.setTracking(true);
    Hector::unitval carbon20(20, Hector::U_PGC);
    TrackedVal y(carbon20, "y");
    y.setTracking(true);
    TrackedVal z = x + y;
    
    cout << "x is " << x << endl;
    cout << "y is " << y << endl;
    cout << "z is " << z << endl;
    z = z - carbon10;
    cout << "z=z-10 is " << z << endl;
    TrackedVal z2 = z / 2;
    cout << "z/2 is " << z2 << endl;
    TrackedVal zpoint5 = z2 * 0.5;
    cout << "z/2*0.5 is " << zpoint5 << endl;
    const TrackedVal test = z;
    TrackedVal twoxz = 2.0 * test;
    cout << "2 * z is " << twoxz << endl;
    
    cout << "\n---------- SIMULATION ----------\n" << endl;
    TrackedVal dest(carbon10, "dest");
    dest.setTracking(true);
    cout << "z = " << z << endl;
    cout << "dest = " << dest << endl;
    for(int i = 0; i < 5; i++) {
        cout << "----------------- i = " << i << endl;
        cout << "z = " << z << endl;
        TrackedVal flux = z * 0.1;
        z = z - flux.get_total();
        dest = dest + flux;
        cout << "flux from z to dest = " << flux << endl;
        cout << "z = " << z << endl;
        cout << "dest = " << dest << endl;
    }
    
    cout << "\n---------- TIMING ----------\n" << endl;
    
    for(int tracking = 0; tracking <= 1; tracking++) {
        x = TrackedVal(carbon10, "x");
        x.setTracking(tracking);
        y = TrackedVal(carbon10, "y");
        y.setTracking(tracking);
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 1000; i++) {
            TrackedVal flux = x * 0.01;
            x = x - flux.get_total();
            y = y + flux;
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "tracking = " << tracking << " time = " << duration.count() << endl;
    }
    
    cout << "\n---------- Time for Tests! ----------\n" << endl;
    test_creation();
    test_tracking();
    test_get_sources();
    test_get_total();
    test_get_fraction();
    test_addition();
    test_subtraction();
    test_mult_div();
    test_equality();
    test_identicality();
}
