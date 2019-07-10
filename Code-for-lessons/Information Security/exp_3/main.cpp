#include <iostream>
using namespace std;

int gcd(int a, int b) {
    int r = a % b;
    if(r) {return gcd(b, r);}
    return b;
}

int ex_gcd(int a, int b, int& u, int& v, int s = 1, int t = 0) {
    int q = a / b;
    int r = a % b;
    if(r) { // r != 0
        int tmp = u;
        u = s - u * q;
        s = tmp;

        tmp = v;
        v = t - v * q;
        t = tmp;

        return ex_gcd(b, r, u, v, s, t);
    }return b;
}

int mod_reverse(int a, int m) {
    int u = 0, v = 1;
    int g = ex_gcd(a, m, u, v);
    if( g != 1 ){
        throw "a 不存在对模 m 的逆";
    }
    return u;
}

int main() {

    int a, m;

    cin >> a >> m;

    try
    {
        cout << mod_reverse(a,m) << endl;
    }
    catch (const char *e)
    {
        cerr << "error: " << e << endl;
        exit(1);
    }    

    return 0;
}