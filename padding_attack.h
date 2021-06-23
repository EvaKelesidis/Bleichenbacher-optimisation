#ifndef PADDING_ATTACK_H_INCLUDED
#define PADDING_ATTACK_H_INCLUDED

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

#include "gmpxx.h"
#include "rsa.h"
#include "interval.h"

using namespace std;

static mpz_class B;

mpz_class mpf_round_to_mpz(mpf_class number);

mpz_class mpf_floor(mpf_class number);

mpz_class mpf_ceil(mpf_class number);

void set_initial_bounds(mpz_class B, mpz_class &left_bound, mpz_class &right_bound);

void change_margins_m( mpz_class &left_bound_m, mpz_class &right_bound_m, mpz_class s, mpz_class alpha, RSA test, mpz_class B);

//mpz_class find_next_s_intervals(mpz_class output, mpz_class s, vector<interval> &M, RSA test, mpz_class B, int &queries);

//mpz_class find_first_s(mpz_class output, mpz_class s, mpz_class &left_bound, mpz_class &right_bound, mpz_class &alpha, RSA test, int &queries, mpz_class B, int &new_bounds);

mpz_class find_next_s(mpz_class output, mpz_class s, RSA test, int &queries, mpz_class B);

//mpz_class minimum_value(mpz_class *m_left_bound, mpz_class *m_right_bound, mpz_class * alpha_in, int ctr, mpz_class offset, RSA test, mpz_class B);

mpz_class minimum_value(vector<interval> M, mpz_class * r, mpz_class offset, RSA test, mpz_class B);

void initialise_interval_list(vector<interval> &M, mpz_class s, mpz_class &left_bound, mpz_class &right_bound, mpz_class &B, RSA test);

void update_intervals(vector<interval> &M, mpz_class s, mpz_class &B, RSA test);

mpz_class find_next_s_intervals_cout(mpz_class output, mpz_class s, vector<interval> &M, RSA test, mpz_class B, int &queries);


#endif // PADDING_ATTACK_H_INCLUDED

