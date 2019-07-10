#include <iostream>  
#include <time.h>
#include <string>  

#include "mpir.h"
#include "gmp.h"
using namespace std;

int main(){
    gmp_randstate_t grt;                  
    gmp_randinit_default(grt);    //随机数生成初始化
    gmp_randseed_ui(grt, time(NULL)); //随机数生成设置种子

    mpz_t p; 
    mpz_t q;
    mpz_init(p);
    mpz_init(q);
    mpz_urandomb(p, grt, 512); //随机生成512位的大整数    
    mpz_urandomb(q, grt, 512); //随机生成512位的大整数 
    mpz_t r; //最大公约数
    mpz_init(r);
    mpz_gcd(r,p,q); //r是p,q的最大公约数

    mpir_ui tmp1=1;  
    mpz_t tmp2;
    mpz_init(tmp2);
    mpz_set_ui(tmp2,tmp1); //tmp2=1; 
    //   mpz_set_str(tmp2,"1",BASE);  // tmp2=1, 使用mpz_set_str可以对mpz_t变量赋初值。

    gmp_printf ("tmp2:%Zd\n", tmp2);
    gmp_printf ("p:%Zd\n", p);
    gmp_printf ("q:%Zd\n", q); 
    gmp_printf ("r:%Zd\n", r); 
    if(mpz_cmp(r,tmp2)==0)
    {

        printf("p,q互素\n");
    }
    else
    {
        printf("p,q不互素\n");
    }
}
