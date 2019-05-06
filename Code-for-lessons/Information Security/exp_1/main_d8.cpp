#include <iostream>
#include <string>
#include "DES.h"
using namespace std;

int main() {

    ElemType key[64] = {
        1, 0, 1, 0, 0, 1, 1, 0,
        1, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 0, 0, 0, 1,
        1, 1, 1, 0, 0, 0, 1, 1,
        0, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0
    };
    ElemType subKeys[16][48];
    DES_MakeSubKeys(key, subKeys);

    ElemType str[8] = {'A','e','o','n','n','i','E','1'};
    cout<<"明文 <str> : ";
    for(int i=0; i < 8; i++) {
        cout << str[i];
    }cout<<endl;

    // 加密明文
    ElemType cstr[8];
    DES_EncryptBlock(str, subKeys, cstr);
    cout<<"密文<cstr> : ";
    for(int i=0; i < 8; i++) {
        cout << cstr[i];
    }cout<<endl;
    
    // 解密密文
    ElemType dstr[8];
    DES_DecryptBlock(cstr, subKeys, dstr);
    cout<<"明文<dstr> : ";
    for(int i=0; i < 8; i++) {
        cout << dstr[i];
    }cout<<endl;

    return 0;
}