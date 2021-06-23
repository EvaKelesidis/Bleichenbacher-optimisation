#ifndef PADDING_ATTACK_CLONE_H_INCLUDED
#define PADDING_ATTACK_CLONE_H_INCLUDED

#include "padding_attack.h"

interval trimming_clone(mpz_class c0, RSA oracle);
interval initialisation_clone(mpz_class c, RSA oracle);

#endif // PADDING_ATTACK_CLONE_H_INCLUDED
