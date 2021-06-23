#include <iostream>
#include <cstdint>
#include <algorithm>
#include <random>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <fstream>

#include "rsa.h"
#include "padding_attack.h"
#include "padding_attack_clone.h"

#include <fstream>
ofstream g;
ofstream f;
using namespace std;


mpz_class find_next_s_intervals(mpz_class output, mpz_class s, vector<interval> &M, RSA test, mpz_class B, int &queries)
{
    mpz_class smallest_m;
    mpz_class left_bound_s, right_bound_s;
    mpz_class s_it;
    mpz_class target;
    mpz_class *smallest_r = new mpz_class[M.size()];
    interval new_interval;
    int i;

    for(int i = 0; i < M.size(); i++)
        smallest_r[i] = 2 * (M[i].b * s - 2 * B)/test.get_n();

    int offset = 0;
    int s_is_good = 0;

    while(!s_is_good)
    {
        //cout << "offset " << dec << offset << endl;
        for(i = 0; i < M.size() && !s_is_good; i++)
        {
            //cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;
           // g << "intervalul lui s " << endl;
           // g << "offset " << offset << endl;
            //g << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << " " << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl;
            for(s_it = (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b; s_it <= (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a && !s_is_good; s_it++)
            {
               // g << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << " " << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl;
                target = (test.encrypt(s_it) * output) % test.get_n();
                if((int)test.decrypt_oracle(target) == 0)
                    {
                        s_is_good = 1;
                        g << "r " << smallest_r[i] + offset << endl;
                        //cout << "interval bun " << endl << hex << M[i].a << endl << M[i].b << endl << endl;
                    }

                queries++;
            }

        }

        offset++;
    }

    delete []smallest_r;
    return s_it - 1;

}

void bleichenbachers(mpz_class output, mpz_class s, RSA test, int &queries)
{
    mpz_class decrypted = test.decrypt(output);
    mpz_class target;
    mpz_class possible_answer;
    mpz_class m=test.decrypt(output);
    mpz_class left_bound;
    mpz_class right_bound;
    int new_bounds = 1;
    mpz_class B;
    B = 0;
    mpz_setbit(B.get_mpz_t(), 8*(test.get_byte_len()-2));


    left_bound = 2*B;
    right_bound = 3*B;

    cout << "2*B " << left_bound << endl;
    cout << "3*B " << right_bound << endl;

    vector<interval> M;

    int update_number = 0;

    initialise_interval_list(M, s, left_bound, right_bound, B, test);

    for(auto it = M.begin(); it != M.end(); it++)
        cout << hex << it->a << endl << it->b << endl << endl;

    s = find_next_s(output, s, test, queries, B);
    g << "\nnext s " << s << endl;
    g << "\nqueries " << queries << endl;
    cout << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;

    cout << endl << endl;

    update_intervals(M, s, B, test);

    for(auto it = M.begin(); it != M.end(); it++)
        cout << hex << it->a << endl << it->b << endl << endl;

    int interval_numbers = 0;

    while(M.size() >= 2)
    {
        s = find_next_s(output, s, test, queries, B);

        cout << "s = " << s << endl;
        cout << "queries " << dec << queries << endl;
        cout << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;
        g << "s = " << s << endl;
        g << "queries " << dec << queries << endl;

        update_intervals(M, s, B, test);

        for(auto it = M.begin(); it != M.end(); it++)
            cout << hex << it->a << endl << it->b << endl << endl;
        cout << endl << endl;
        interval_numbers = interval_numbers + 1;
    }

    g << "iteration number " << interval_numbers << endl;
    g << "queries until now " << queries << endl;


    while(M[0].b - M[0].a > 1)
    {

        s = find_next_s_intervals(output, s, M, test, B, queries);

        cout << "s = " << s << endl;
        cout << "queries " << dec << queries << endl;
        cout << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;
        g << "s = " << s << endl;
        g << "queries " << dec << queries << endl;

        update_intervals(M, s, B, test);

        for(auto it = M.begin(); it != M.end(); it++)
            cout << hex << it->a << endl << it->b << endl << endl;
        cout << endl << endl;
    }

    cout << M[0].a << endl << M[0].b << endl;
    if(test.encrypt(M[0].a) == output || test.encrypt(M[0].b) == output)
        {
            g << "Good decryption! :)" << endl;
            f << "Good decryption! :)" << endl;
            cout << "Good decryption! :)" << endl;
        }
    cout << decrypted << endl;
    cout << dec << queries << endl;
    g << dec << queries << endl;
    f << "queries bleichenbacher " << queries << endl;

}

void bleichenbachers_1(mpz_class output,mpz_class s, RSA test, int &queries)
{
    mpz_class decrypted = test.decrypt(output);
    mpz_class target;
    mpz_class possible_answer;
    mpz_class m=test.decrypt(output);
    mpz_class left_bound;
    mpz_class right_bound;
    int new_bounds = 1;
    mpz_class B;
    B = 0;
    mpz_setbit(B.get_mpz_t(), 8*(test.get_byte_len()-2));

    left_bound = 2*B;
    right_bound = 3*B;

    cout << "2*B " << left_bound << endl;
    cout << "3*B " << right_bound << endl;

    vector<interval> M;

    int update_number = 0;

    initialise_interval_list(M, s, left_bound, right_bound, B, test);

    for(auto it = M.begin(); it != M.end(); it++)
        cout << hex << it->a << endl << it->b << endl << endl;

    s = find_next_s_intervals(output, s, M, test, B, queries);
    g << "\nnext s " << s << endl;
    g << "\nqueries " << queries << endl;
    cout << "validation check " << ((2*B < (decrypted * s) % test.get_n()) && ((decrypted * s) % test.get_n() < 3*B)) << endl;
    g << "validation check " << ((2*B < (decrypted * s) % test.get_n()) && ((decrypted * s) % test.get_n() < 3*B)) << endl;

    cout << endl << endl;

    update_intervals(M, s, B, test);

    for(auto it = M.begin(); it != M.end(); it++)
        cout << hex << it->a << endl << it->b << endl << endl;

    int interval_numbers = 0;

    while(M.size() >= 2)
    {
        s = find_next_s_intervals(output, s, M, test, B, queries);

        cout << "s = " << s << endl;
        cout << "queries " << dec << queries << endl;
        cout << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;
        g << "s = " << s << endl;
        g << "queries " << dec << queries << endl;
        g << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;


        update_intervals(M, s, B, test);

        for(auto it = M.begin(); it != M.end(); it++)
            cout << hex << it->a << endl << it->b << endl << endl;
        cout << endl << endl;
        interval_numbers = interval_numbers + 1;
    }

    g << "number of iterations " << interval_numbers << endl;
    g << "queries until now " << queries << endl;

    while(M[0].b - M[0].a > 1)
    {
        s = find_next_s_intervals(output, s, M, test, B, queries);

        cout << "s = " << s << endl;
        cout << "queries " << dec << queries << endl;
        cout << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;
        g << "s = " << s << endl;
        g << "queries " << dec << queries << endl;
        g << "validation check " << (2*B < (decrypted * s) % test.get_n() && (decrypted * s) % test.get_n() < 3*B) << endl;

        update_intervals(M, s, B, test);

        for(auto it = M.begin(); it != M.end(); it++)
            cout << hex << it->a << endl << it->b << endl << endl;
        cout << endl << endl;

    }

    cout << M[0].a << endl << M[0].b << endl;
    if(test.encrypt(M[0].a) == output || test.encrypt(M[0].b) == output)
        {
            g << "Good decryption! :)" << endl;
            f << "Good decryption! :)" << endl;
            cout << "Good decryption! :)" << endl;
        }
    cout << decrypted << endl;
    cout << dec << queries << endl;
    g << dec << queries << endl;
    f << "queries E " << queries << endl;


}

void tests(uint8_t* in, uint32_t in_len, RSA *test, int &queries_bl1, int &queries_bl)
{

    mpz_class output = test->encrypt(in, in_len);
    mpz_class m=test->decrypt(output);

    cout << hex << " n = " << test->get_n() << endl << endl << " m = " << m << endl << endl;
    g << hex << " n = " << test->get_n() << endl << endl << " m = " << m << endl << endl;
    f << hex << " n = " << test->get_n() << endl << endl << " m = " << m << endl << endl;

    cout << hex << " d = " << test->get_d() << endl << endl << " e = " << test->get_e() << endl << endl << flush;
    g << hex << " d = " << test->get_d() << endl << endl << " e = " << test->get_e() << endl << endl << flush;
    f << hex << " d = " << test->get_d()<< endl << endl << " e = " << test->get_e() << endl << endl << flush;

    mpz_class B;
    B = 0;
    mpz_setbit(B.get_mpz_t(), 8*(test->get_byte_len()-2));

    mpz_class s1 = 1 + (test->get_n())/(3 * B);
    int queries=0;
    int queries_remainder = 0;

    mpz_class left_bound = 2*B;
    mpz_class right_bound = 3*B;
    mpz_class target;

    cout << "2*B " << left_bound << endl;
    cout << "3*B " << right_bound << endl;
    int flag_final = 0;

    do{
        s1++;
        queries++;

        if(queries > 20000000)
        {
            flag_final = 1;
            break;
        }

        target = (output * test->encrypt(s1)) % test->get_n();
    }while((int)(test->decrypt_oracle(target)) != 0);

    if(flag_final == 1 )
        return;

    mpz_class s = s1;
    queries_remainder = queries;

    f << "\nprimul s gasit " << s << endl;
    f << "numarul de queryuri " << queries << endl;

    bleichenbachers_1(output, s1, *test, queries);

    cout << " queries E = "<< dec<<queries <<hex<<endl<<endl<<flush;
    g << " queries E = "<< dec<<queries <<hex<<endl<<endl<<flush;
    f << " queries E = "<< dec<<queries <<hex<<endl<<endl<<flush;


    queries_bl1 = queries;
    queries = queries_remainder;

    bleichenbachers(output, s, *test, queries);

    cout << " queries Bleichenbachers = "<< dec<<queries <<hex<<endl<<endl<<flush;
    g << " queries Bleichenbachers = "<< dec<<queries <<hex<<endl<<endl<<flush;
    f << " queries Bleichenbachers = "<< dec<<queries <<hex<<endl<<endl<<flush;
    queries_bl = queries;

}


int main()
{


    return 0;
}
