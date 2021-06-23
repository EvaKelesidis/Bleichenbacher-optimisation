#include "padding_attack_clone.h"

void generate_pairs_clone(mpz_class c0, RSA oracle, mpz_class bound , vector<mpz_class> &t_set)
{
    //cateva mii conform articol
    for(uint32_t i = 0; i < 5000; )
    {
        mpz_class t, u, flag;

        do t = prng.get_z_range(bound);
        while(t == 0);

        do
        {
            u = prng.get_z_range(1.5*t);
            mpz_gcd(flag.get_mpz_t(), u.get_mpz_t(), t.get_mpz_t());
        }
        while(flag != 1);

        if(oracle.decrypt_oracle(compute_test_enc_trim(c0, u, t, oracle)) == 0)
        {
            t_set.push_back(t);
            i++;
        }
    }
}

//de optimizat
mpz_class lcm_set(vector<mpz_class> t_set)
{
    mpz_class lcm_t = t_set[0];

    for(uint32_t i = 1; i < t_set.size(); i++)
        mpz_lcm(lcm_t.get_mpz_t(), lcm_t.get_mpz_t(), t_set[i].get_mpz_t());

    return lcm_t;
}

interval trimming_clone(mpz_class c0, RSA oracle)
{
    vector<mpz_class> t_set;
    mpz_class B = 0, bound, lcm_t;
    mpf_class trim_min, trim_max;
    interval M0;

    mpz_setbit(B.get_mpz_t(), 8*(oracle.get_byte_len()-2));
    bound = (2*oracle.get_n())/(9*B);

    generate_pairs_clone(c0, oracle, bound, t_set);
    lcm_t = lcm_set(t_set);

    for(mpz_class u = 1; u < 1.5*lcm_t; u++)
    {
        if(oracle.decrypt_oracle(compute_test_enc_trim(c0, u, lcm_t, oracle)) == 0)
            {
                trim_max = mpf_class(lcm_t)/mpf_class(u);
                break;
            }
    }

    for(mpz_class u = 1.5*lcm_t -1 ; u > 0; u--)
    {
        if(oracle.decrypt_oracle(compute_test_enc_trim(c0, u, lcm_t, oracle)) == 0)
            {
                trim_min = mpf_class(lcm_t)/mpf_class(u);
                break;
            }
    }

    M0.a = 2*B*trim_max;
    M0.b = (3*B-1)*trim_min;

    if(M0.a <= oracle.decrypt(c0) && oracle.decrypt(c0) <= M0.b) cout << "ok" << endl;
    else cout << "nok" << endl;

    return M0;
}

interval initialisation_clone(mpz_class c, RSA oracle)
{
    mpz_class c0 = blinding(c, oracle);

    cout << "blinding end." << endl;

    return trimming_clone(c0, oracle);
}
