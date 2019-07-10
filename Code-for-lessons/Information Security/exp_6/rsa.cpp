#include "rsa.h"
#include "libs/base64.h"

// 随机生成一个素数
void get_prime(mpz_t prime, gmp_randstate_t grt, int length) {                                            
    mpz_t p; mpz_init(p);
    mpz_urandomb(p, grt, length); //随机生成length位的大整数             
    mpz_nextprime(prime, p);  //使用GMP自带的素数生成函数  
    mpz_clear(p);   
}

// 计算 phi_n 的值
void calc_phiN(mpz_t phi_n, mpz_t p, mpz_t q) {
    mpz_t p_1; mpz_init(p_1);
    mpz_t q_1; mpz_init(q_1);
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(phi_n, p_1, q_1);
    mpz_clear(p_1);
    mpz_clear(q_1);
}

// 随机生成整数 e
void get_e(mpz_t e, gmp_randstate_t grt, mpz_t phi_n) {
    mpz_t r; mpz_init(r); //最大公约数
    int lo = 100;
    while(lo--){
        mpz_urandomm(e, grt, phi_n);
        mpz_gcd(r, e, phi_n);
        if(mpz_cmp_ui(r,1) == 0 && mpz_cmp_ui(e,0) != 0) {
            break;
        }
    }
    if (lo <= 0) {cout << "Get-E-Error!" << endl;}
}


void gen_keys(mpz_t p, mpz_t q, mpz_t n, mpz_t e, mpz_t d, size_t length) {
    // 随机数生成初始化, 随机数生成设置种子
    gmp_randstate_t grt;                  
    gmp_randinit_default(grt);    
    gmp_randseed_ui(grt, time(NULL)); 

    // 随机生成两个大素数
    get_prime(p, grt, length/2);
    get_prime(q, grt, length/2);

    // 计算 n = pq
    mpz_mul(n, p, q);

    // 计算phi_n的值 = (p-1)(q-1)
    mpz_t phi_n; mpz_init(phi_n);
    calc_phiN(phi_n, p, q);

    // 随机生成整数 e
    get_e(e, grt, phi_n);

    // 计算 d = e^-1 mod phi_n
    while(!mpz_invert(d, e, phi_n)) {cout << "Error computing invert!" << endl;}
    mpz_clear(phi_n);
}

int test(size_t length) {

    cout << "程序RSA加解密测试..." << endl;
    cout << "RSA 模数：" << length << "bits" << endl << endl;
    cout << "正在生成加解密密钥..." << endl << endl;

    mpz_t p; mpz_init(p);
    mpz_t q; mpz_init(q);
    mpz_t n; mpz_init(n);
    mpz_t e; mpz_init(e);
    mpz_t d; mpz_init(d);

    string s ;
    size_t strsize;

    gen_keys(p, q, n, e, d, length);

    cout << "\033[31m公开部分: \033[0m" << endl;
    gmp_printf ("\033[36mn (10进制):\033[0m\n%Zd\n", n);
    s = base64_encode(to_string(n, &strsize), strsize); 
    cout << "\033[36mn (Base64):\033[0m" << endl << s << endl;
    gmp_printf ("\033[36m加密公钥 (10进制):\033[0m\n%Zd\n", e);  
    s = base64_encode(to_string(e, &strsize), strsize); 
    cout << "\033[36m加密公钥 (Base64):\033[0m" << endl << s << endl;


    cout << "\n\033[31m私密部分: \033[0m" << endl;
    gmp_printf ("\033[36mp (10进制):\033[0m\n%Zd\n", p);
    s = base64_encode(to_string(p, &strsize), strsize); 
    cout << "\033[36mp (Base64):\033[0m" << endl << s << endl;
    gmp_printf ("\033[36mq (10进制):\033[0m\n%Zd\n", q); 
    s = base64_encode(to_string(q, &strsize), strsize); 
    cout << "\033[36mq (Base64):\033[0m" << endl << s << endl;
    gmp_printf ("\033[36m解密私钥 (10进制):\033[0m\n%Zd\n", d);
    s = base64_encode(to_string(d, &strsize), strsize); 
    cout << "\033[36m解密私钥 (Base64):\033[0m" << endl << s << endl; 

    string str;
    cout << "\n\033[36m请输入测试的字符串（小于长度" << length/8-1 << "字节）: \033[0m";
    cin >> str;
    if(str.length() > length/8-1) {
        cout << "字符串长度超出！" << str.length() << endl;
        return -1;
    }

    mpz_t c; mpz_init(c);
    mpz_t m; mpz_init(m);
    mpz_t de; mpz_init(de);

    from_string(m, str);
    gmp_printf ("\n\033[36m字符串对应的数字 (10进制):\033[0m\n%Zd\n", m); 

    mpz_powm(c, m, e, n);
    gmp_printf ("\033[36m加密后的密文 (10进制):\033[0m\n%Zd\n", c); 

    mpz_powm(de, c, d, n);
    gmp_printf ("\033[36m解密后的字符串数字 (10进制):\033[0m\n%Zd\n", de); 

    s = to_string(de);
    cout << "\033[36m解密后的字符串:\033[0m\n" << s << endl;

    cout << "\n\033[36m完成！\033[0m\n" << endl;

    return 0;
}