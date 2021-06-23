#include "rsa.h"

RSA::RSA(uint32_t n_byte_len)
{
    mpz_class phi, flag;
    uint32_t prime_bit_size;
    mpz_class n_max;

    mpz_class range = 3;
    mpz_setbit(n_max.get_mpz_t(), 8*(n_byte_len)-4);

    do{
    mpz_class rest = prng.get_z_range(range);

    this->n_byte_len = n_byte_len;
    prime_bit_size = 4*(n_byte_len-1) + rest.get_ui();

    p = prng.get_z_bits(prime_bit_size);
    mpz_setbit(p.get_mpz_t(), prime_bit_size);
    mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());

    rest = prng.get_z_range(range);

    //cout<<"rest 2 = "<<rest<<endl;

    prime_bit_size = 4*(n_byte_len-1) + rest.get_ui();

    q = prng.get_z_bits(prime_bit_size);
    mpz_setbit(q.get_mpz_t(), prime_bit_size);
    mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

    n = p*q;}
    while(n > n_max);
    phi = (p-1)*(q-1);

    e = 3;
    mpz_gcd(flag.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    if(flag != 1) e = 65537;
    mpz_gcd(flag.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    if(flag != 1) throw "Exponent e invalid";

    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

}

RSA::RSA(RSA &input)
{
    this->p = input.get_p();
    this->q = input.get_q();
    this->n = input.get_n();
    this->e = input.get_e();
    this->d = input.get_d();
    this->n_byte_len = input.get_byte_len();
}

RSA::RSA(mpz_class n, mpz_class e, mpz_class d, uint32_t n_byte_len)
{
    this->n = n;
    this->e = e;
    this->d = d;
    this->n_byte_len = n_byte_len;

}

RSA::RSA(mpz_class n, mpz_class e, uint32_t n_byte_len)
{
    this->n = n;
    this->e = e;
    this->n_byte_len = n_byte_len;
}

mpz_class RSA::convert_to_mpz(uint8_t *input, uint32_t in_byte_len)
{
    mpz_class output = 0;

    for(uint32_t i = 0; i < in_byte_len; i++)
        output = (output << 8) | input[i];

    return output;
}

void RSA::convert_to_chars(mpz_class input, uint8_t *output, uint32_t out_byte_len)
{
    for(uint32_t i = 0; i < out_byte_len; i++)
    {
        uint32_t offset = 8*(out_byte_len - 1 - i);
        mpz_class byte = (input >> offset) & 0xff;
        output[i] = byte.get_ui();
    }
}

mpz_class RSA::add_padding(uint8_t *input, uint32_t in_byte_len)
{
    if(in_byte_len >= n_byte_len-11) throw "Input length too big.";

    uint8_t *pad_data = new uint8_t [n_byte_len] {0};
    mpz_class output;
    uint32_t ps_byte_len = n_byte_len-3-in_byte_len;

    pad_data[0] = 0x00;
    pad_data[1] = 0x02;

    for(uint32_t i = 0; i < ps_byte_len;)
    {
        mpz_class byte = prng.get_z_bits(8);

        if(byte != 0x00)
        {
            pad_data[i+2] = byte.get_ui();
            i++;
        }
    }

    //pad_data[50] = 0x00;

    pad_data[ps_byte_len+2] = 0x00;

    for(uint32_t i = 0; i < in_byte_len; i++)
        pad_data[i+ps_byte_len+3] = input[i];

    output = convert_to_mpz(pad_data, n_byte_len);

    delete[] pad_data;

    return output;
}
//
//uint8_t RSA::check_padding(uint8_t *input)
//{
//    if(input[0] != 0x00 || input[1] != 0x02) return 1;
//
//    for(uint32_t i = 2; i < 10; i++)
//        if(input[i] == 0x00) return 1;
//
//   for(uint32_t i = 10; i < n_byte_len; i++)
//        if(input[i] == 0) return 0;
//
//    return 1;
//}


uint8_t RSA::check_padding(uint8_t *input)
{
    if(input[0] != 0x00 || input[1] != 0x02) return 1;

    int zero_index = 2;
    for(uint32_t i = 2; i < n_byte_len; i++)
        if(input[i] == 0x00){
            zero_index = i;
            break;
        }
    if(zero_index > n_byte_len - 11 || zero_index < n_byte_len - 65)
        return 1;

   /*if(zero_index != n_byte_len - 49)
        return 1;*/

    return 0;
}

uint8_t* RSA::remove_padding(uint8_t *input, uint32_t &out_byte_len)
{
    uint32_t i = 2, offset;
    uint8_t *output;

    while(input[i] != 0x00) i++;

    offset = i + 1;
    out_byte_len = n_byte_len - offset;

    output = new uint8_t [out_byte_len] {0};

    for(i = offset; i < n_byte_len; i++)
        output[i-offset] = input[i];

    return output;
}

mpz_class RSA::encrypt(mpz_class input)
{
    mpz_class output;

    mpz_powm(output.get_mpz_t(), input.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

    return output;
}

mpz_class RSA::encrypt(uint8_t *input_data, uint32_t in_byte_len)
{
    mpz_class plain = add_padding(input_data, in_byte_len);

//    cout << hex << plain << endl;

    return encrypt(plain);
}

mpz_class RSA::decrypt(mpz_class input)
{
    mpz_class output;

    mpz_powm(output.get_mpz_t(), input.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

    return output;
}

uint8_t* RSA::decrypt(mpz_class input, uint32_t &out_byte_len)
{
    mpz_class dec = decrypt(input);
    uint8_t *dec_data = new uint8_t [n_byte_len] {0};
    uint8_t *output;

    convert_to_chars(dec, dec_data, n_byte_len);
    if(check_padding(dec_data) != 0) throw "Padding incorect.";
    output = remove_padding(dec_data, out_byte_len);

    delete[] dec_data;

    return output;
}

uint8_t RSA::decrypt_oracle(mpz_class input)
{
    mpz_class dec = decrypt(input);
    uint8_t *dec_data = new uint8_t [n_byte_len] {0};

    convert_to_chars(dec, dec_data, n_byte_len);

    //for(int i = 0; i < n_byte_len; i++)
        //cout << hex << (int)dec_data[i] << " ";

   // cout << endl;

    uint8_t flag = check_padding(dec_data);

    delete[] dec_data;

    return flag;
}

uint32_t RSA::get_byte_len()
{
    return this->n_byte_len;
}

mpz_class RSA::get_e()
{
    return this->e;
}

mpz_class RSA::get_n()
{
    return this->n;
}

mpz_class RSA::get_d()
{
    return this->d;
}

mpz_class RSA::get_p()
{
    return this->p;
}

mpz_class RSA::get_q()
{
    return this->q;
}


