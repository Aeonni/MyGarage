#ifndef ENCODING_H
#define ENCODING_H

#include <iostream>  
#include <time.h>
#include <fstream>
#include <string>  
#include "rsa.h"
using namespace std;

void to_file(const char * filename, mpz_t data);

void from_string(mpz_t data, string str);

char * to_string(mpz_t data, size_t * s = NULL);

#endif