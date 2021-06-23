#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include <iostream>
using namespace std;

#include "gmpxx.h"
#include "common.h"

class RSA
{
public:
    mpz_class p, q, n, e, d;
    uint32_t n_byte_len;

    mpz_class convert_to_mpz(uint8_t *input, uint32_t in_byte_len);
//    void convert_to_chars(mpz_class input, uint8_t *output, uint32_t out_byte_len);

    mpz_class add_padding(uint8_t *input, uint32_t in_byte_len);
//    uint8_t check_padding(uint8_t *input);
    uint8_t* remove_padding(uint8_t *input, uint32_t &out_byte_len);
    RSA(uint32_t n_byte_len);
    RSA(RSA &input);
    RSA(mpz_class n, mpz_class e, mpz_class d, uint32_t n_byte_len);
    RSA(mpz_class n, mpz_class e, uint32_t n_byte_len);

    mpz_class encrypt(mpz_class input); //textbook
    mpz_class encrypt(uint8_t *input_data, uint32_t in_byte_len); //pkcs#1 v1.5

    mpz_class decrypt(mpz_class input); //textbook
    uint8_t* decrypt(mpz_class input, uint32_t &out_byte_len); //pkcs#1 v1.5

    uint8_t decrypt_oracle(mpz_class input);

    uint32_t get_byte_len();
    mpz_class get_e();
    mpz_class get_d();
    mpz_class get_n();
    mpz_class get_p();
    mpz_class get_q();

    void convert_to_chars(mpz_class input, uint8_t *output, uint32_t out_byte_len);
    uint8_t check_padding(uint8_t *input);
    uint8_t check_padding_hard(uint8_t *input);
};

#endif // RSA_H_INCLUDED
