
#include "padding_attack.h"

mpz_class set_B(RSA oracle)
{
    B = 0;
    mpz_setbit(B.get_mpz_t(), 8*(oracle.get_byte_len()-2));
    return B;
}


mpz_class find_next_s(mpz_class output, mpz_class s, RSA test, int &queries, mpz_class B)
{
    mpz_class target;
    int flag_good_s = 0;
    s = s + 1;

    while(!flag_good_s)
    {
        target = (output * test.encrypt(s)) % test.get_n();
        if((int)(test.decrypt_oracle(target)) == 0)
            flag_good_s=1;
        queries++;
        s++;
    }

    return s - 1;
}
/*
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
            g << "intervalul lui s " << endl;
            for(s_it = (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b; s_it <= (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a && !s_is_good; s_it++)
            {
                g << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << " " << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl;
                target = (test.encrypt(s_it) * output) % test.get_n();
                if((int)test.decrypt_oracle(target) == 0)
                    {
                        s_is_good = 1;
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

*/
mpz_class find_next_s_intervals_cout(mpz_class output, mpz_class s, vector<interval> &M, RSA test, mpz_class B, int &queries)
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
            cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;
            for(s_it = (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b; s_it < (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a && !s_is_good; s_it++)
            {
                target = (test.encrypt(s_it) * output) % test.get_n();
                if((int)test.decrypt_oracle(target) == 0)
                    {
                        s_is_good = 1;
                        //cout << "interval bun " << endl << hex << M[i].a << endl << M[i].b << endl << endl;
                    }

                queries++;
            }

        }

        offset++;
    }

    //cout << "s = " << s_it - 1 << endl;
    //cout << "queries " << queries << endl;

   // cout << "interval bun " << endl << hex << M[i - 1].a << endl << M[i - 1].b << endl << endl;

    //cout << "interval s " << endl << hex << (2 * B + (smallest_r[i - 1] + offset - 1) * test.get_n())/M[i - 1].b << endl << (3 * B + (smallest_r[i - 1] + offset - 1)*test.get_n())/M[i - 1].a << endl << endl;

    /*cout << "offset " << dec << offset << endl;

    for(i = 0; i < M.size(); i++)
        cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;

    offset++;

    cout << "offset " << dec << offset << endl;

    for(i = 0; i < M.size(); i++)
        cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;


    offset++;

    cout << "offset " << dec << offset << endl;

    for(i = 0; i < M.size(); i++)
        cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;

    offset++;

    cout << "offset " << dec << offset << endl;

    for(i = 0; i < M.size(); i++)
        cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;

    offset++;

    cout << "offset " << dec << offset << endl;

    for(i = 0; i < M.size(); i++)
        cout << "s interval number" << dec << i << endl << hex << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset)*test.get_n())/M[i].a << endl << endl;
*/

    //cout << "interval bun " << endl << hex << M[i - 1].a << endl << M[i - 1].b << endl << endl;

    // salvam marginile lui m corespunzatoare s-ului, cautam margini care ar mai putea corespunde lui s
    // pastram marginea inferioara a lui s la care am ajuns
    // cautam s-uri cat timp marginea este mai mare decat minimul marginilor inferioare ale intervalelor 1lui s
    //new_interval.set_interval(M[i - 1].a, M[i - 1].b);
    //M.clear();

    //vector <interval> M_new;
    //M_new.insert(M_new.begin(), new_interval);

    //cout << "intervalul lui s " << endl << (2 * B + (smallest_r[i-1] + offset-1) * test.get_n())/M[i-1].b << endl << (3 * B + (smallest_r[i-1] + offset-1) * test.get_n())/M[i-1].a << endl << endl;
/*
    mpz_class minimum_s;
    right_bound_s = (3 * B + (smallest_r[i-1] + offset-1) * test.get_n())/M[i-1].a;
    minimum_s = minimum_value(M, smallest_r, offset, test, B);
    while(right_bound_s > minimum_s)
    {
        for(i = 0; i < M.size(); i++)
        {
            //cout << "limite pentru s " << dec << i << hex << endl << (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b << endl << (3 * B + (smallest_r[i] + offset) * test.get_n())/M[i].a << endl << endl;
            if(s_it - 1 >= (2 * B + (smallest_r[i] + offset) * test.get_n())/M[i].b && s_it - 1 <= (3 * B + (smallest_r[i] + offset) * test.get_n())/M[i].a)
            {

                new_interval.set_interval(M[i].a, M[i].b);
                //cout << "Interval bun " << M[i].a << endl << M[i].b << endl << endl;
                M_new.insert(M_new.begin(), new_interval);

            }

        }

        offset++;
        minimum_s = minimum_value(M, smallest_r, offset, test, B);

    }


   // for(auto it = M_new.begin(); it != M_new.end(); it++)
     //   cout << it->a << endl << it->b << endl << endl;

    if(M_new.size() == M.size())
        return s_it - 1;

    M.clear();

    for(i = 0; i < M_new.size(); i++)
        M.insert(M.begin() + i, M_new[i]);
*/
    return s_it - 1;

}



 mpz_class minimum_value(vector<interval> M, mpz_class * r, mpz_class offset, RSA test, mpz_class B)
 {
    mpz_class minimum = (2*B + (r[0] + offset) * test.get_n()) / M[0].b;
    for(int i = 1; i < M.size(); i++)
        if((2*B + (r[i] + offset) * test.get_n()) / M[i].b < minimum)
            minimum = (2*B + (r[i] + offset) * test.get_n()) / M[i].b;

    return minimum;
 }
/*
 mpz_class minimum_value(mpz_class *m_left_bound, mpz_class *m_right_bound, mpz_class * alpha_in, int ctr, mpz_class offset, RSA test, mpz_class B)
 {
    mpz_class minimum = (2*B + (alpha_in[0] + offset) * test.get_n()) / m_right_bound[0];
    for(int i = 1; i < ctr; i++)
        if((2*B + (alpha_in[i] + offset) * test.get_n()) / m_right_bound[i] < minimum)
            minimum = (2*B + (alpha_in[i] + offset) * test.get_n()) / m_right_bound[i];

    return minimum;
 }
*/





 void update_intervals(vector<interval> &M, mpz_class s, mpz_class &B, RSA test)
 {
    mpz_class left_bound_m, right_bound_m;
    mpz_class left_bound_r, right_bound_r;
    interval new_interval;
    for(int i = 0; i < M.size(); i++)
    {
        left_bound_r = (M[i].a * s - 3 * B + 1)/test.get_n() + 1;
        right_bound_r = (M[i].b * s - 2 * B)/test.get_n();
        //cout << "r bounds " << i << endl << left_bound_r << endl << right_bound_r << endl << endl;
        if(left_bound_r > right_bound_r)
        {
            M.erase(M.begin() + i, M.begin() + i + 1);
            continue;
        }

        for(mpz_class r = left_bound_r; r <= right_bound_r; r++)
        {
            if(M[i].a > (2 * B + r * test.get_n())/s + 1)
                left_bound_m = M[i].a;
            else
                left_bound_m = (2 * B + r * test.get_n())/s + 1;

            if(M[i].b < (3 * B - 1 + r * test.get_n())/s)
                right_bound_m = M[i].b;
            else
                right_bound_m = (3 * B - 1 + r * test.get_n())/s;

            // cout << "m bound " << i << endl << left_bound_m << endl << right_bound_m << endl << endl;

            new_interval.set_interval(left_bound_m, right_bound_m);
            break_an_interval(M, new_interval);
        }
    }

 }


void initialise_interval_list(vector<interval> &M, mpz_class s, mpz_class &left_bound, mpz_class &right_bound, mpz_class &B, RSA test)
{
    mpz_class left_bound_m, right_bound_m;
    mpz_class left_bound_r, right_bound_r;
    interval new_interval;

    left_bound_r = (2* B * s - 3 * B + 1)/test.get_n() + 1;
    right_bound_r = ((3 * B - 1) * s - 2 * B)/test.get_n();

    for(mpz_class r = left_bound_r; r <= right_bound_r; r++)
    {
        if(2 * B > (2 * B + r * test.get_n())/s + 1)
            left_bound_m = 2 * B;
        else
            left_bound_m = (2 * B + r * test.get_n())/s;

        if(3 * B - 1 < (3 * B - 1 + r * test.get_n())/s)
            right_bound_m = 3 * B - 1;
        else
            right_bound_m = (3 * B - 1 + r * test.get_n())/s;

        new_interval.set_interval(left_bound_m, right_bound_m);
        M.insert(M.begin(), new_interval);
        //break_an_interval(M, new_interval);


    }



}
