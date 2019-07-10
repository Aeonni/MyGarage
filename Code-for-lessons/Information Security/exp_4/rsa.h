#ifndef RSA_H
#define RSA_H

#include "mpir.h"
#include "gmp.h"
#include "encoding.h"
#include <iostream>  
using namespace std;

void get_prime(mpz_t prime, gmp_randstate_t grt, int length);

void calc_phiN(mpz_t phi_n, mpz_t p, mpz_t q);

void get_e(mpz_t e, gmp_randstate_t grt, mpz_t phi_n);

void gen_keys(mpz_t p, mpz_t q, mpz_t n, mpz_t e, mpz_t d, size_t length);

int test(size_t length = 2048);

#endif