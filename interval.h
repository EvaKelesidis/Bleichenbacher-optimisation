#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <vector>
#include <algorithm>

#include "gmpxx.h"

using namespace std;

struct interval
{
    mpz_class a, b;

    interval() : a(0), b(0){};
    interval(mpz_class a,mpz_class b)
    {
        if(a > b) throw "Interval invalid.";

        this->a = a;
        this->b = b;
    }

    bool in(mpz_class value);
    mpz_class size();

    friend bool operator == (interval ab, interval cd);
    friend bool operator < (interval ab, interval cd);
    friend bool operator <= (interval ab, interval cd);
    void set_interval(mpz_class a, mpz_class b);
};

interval reunion(interval ab, interval cd);
void multiple_reunion(vector<interval> &input);
void safe_interval_insert(vector <interval> &M_new, interval in);
void break_an_interval(vector <interval> &M_new, interval in);

#endif // INTERVAL_H_INCLUDED
