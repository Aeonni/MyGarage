#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "DES.h"
using namespace std;
#define BufferSIZE 1024*1024 //512k

ElemType key[64] = {
        0, 1, 1, 1, 1, 0, 0, 0,
        1, 0, 1, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 1, 1,
        1, 0, 0, 0, 1, 1, 0, 0,
        0, 0, 1, 1, 1, 0, 1, 1,
        1, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 0, 0, 1, 0
};
ElemType subKeys[16][48];


ElemType group[8];
ElemType cstr[8] = {1, 1, 1, 1, 1, 1, 1, 1};
ElemType dstr[8];
ElemType oldgroup[8] = {1, 1, 1, 1, 1, 1, 1, 1};

long fsize = 0;
int bias = 0;
long buff_p = 0;
long total_buff = 0;
int p = 0;
int cusize = 0;

ElemType *buff = (ElemType *)malloc(BufferSIZE);


void XOR(ElemType in[8], ElemType overwrite[8]) {
    for(int i=0; i < 8; i++) {
        overwrite[i] = (in[i]^overwrite[i]);
    }
}

void encrypt(string filename, string enfilename) {
    DES_MakeSubKeys(key, subKeys);
    fsize = 0;
    bias = 0;
    buff_p = 0;
    total_buff = 0;
    p = 0;
    cusize = 0;

    ifstream rfile;
    rfile.open(filename, ios::in | ios::binary);
    ofstream cfile;
    cfile.open(enfilename, ios::out | ios::trunc | ios::binary);

    rfile.seekg(0, rfile.end);
    fsize = rfile.tellg();
    if(!fsize) {
        cout << "无法读取文件!" << endl;
        return;
    }
    rfile.seekg(0, ios::beg);

    bias = 8-fsize%8;
    for(int i = 0; i<8; i++) {
        cfile << (char)bias;
    }
    
    cout << "正在加密...文件大小: " << fsize << 'B' << endl; 
    cout << "[.."<<flush;
    do {
        for(int i=0; i < 8; i++) {
            group[i] = rfile.get();
        }
        XOR(cstr, group);
        DES_EncryptBlock(group, subKeys, cstr);
        for(int i=0; i < 8; i++) {
            *(buff+buff_p) = cstr[i];
            buff_p += 1;
        }
        // 当缓存满了就写入磁盘
        if(buff_p == BufferSIZE){
            cfile.write(buff, BufferSIZE);
            buff_p = 0;
            total_buff += BufferSIZE;
        }

        // 进度条
        cusize = (rfile.tellg()*100/fsize);
        if(cusize%10 == 0 && cusize > p) {
            p += 10;
            cout<< p <<"%.." <<flush;
        }

    } while(rfile.tellg()>0);
    cfile.write(buff, buff_p);
    cout << "100%]" << endl;
    cout << "加密完成!  加密大小: " << cfile.tellp() << 'B' << endl;
    rfile.close();
    cfile.close();
}


void decrypt(string enfilename, string defilename) {
    DES_MakeSubKeys(key, subKeys);

    fsize = 0;
    bias = 0;
    buff_p = 0;
    total_buff = 0;
    p = 0;
    cusize = 0;

    ifstream enfile;
    enfile.open(enfilename, ios::in | ios::binary);
    ofstream dfile;
    dfile.open(defilename, ios::out | ios::trunc | ios::binary);

    enfile.seekg(0, enfile.end);
    fsize = enfile.tellg();
    if(!fsize) {
        cout << "无法读取文件!" << endl;
        return;
    }
    enfile.seekg(0, ios::beg);

    for(int i=0; i < 8; i++) {
        bias = enfile.get();
    }
    
    cout << "正在解密...文件大小: " << fsize << 'B' << endl;
    fsize -= (bias+8);
    cout << "[.."<<flush;
    do {
        for(int i=0; i < 8; i++) {
            group[i] = enfile.get();
        }
        DES_DecryptBlock(group, subKeys, dstr);
        XOR(oldgroup, dstr);
        for(int i=0; i < 8; i++) {
            *(buff+buff_p) = dstr[i];
            oldgroup[i] = group[i];
            buff_p += 1;
        }
        // 当缓存满了就写入磁盘
        if(buff_p == BufferSIZE){
            dfile.write(buff, BufferSIZE);
            buff_p = 0;
            total_buff += BufferSIZE;
        }

        // 进度条
        cusize = (enfile.tellg()*100/fsize);
        if(cusize%10 == 0 && cusize > p) {
            p += 10;
            cout<< p <<"%.." <<flush;
        }

    } while(enfile.tellg()>0);
    dfile.write(buff, fsize-total_buff);
    cout << "100%]" << endl;
    cout << "解密完成!  解密大小: " << dfile.tellp() << 'B' << endl;
    enfile.close();
    dfile.close();
}

int main(int argc, char * argv[]) {
    string ifile;
    string ofile;
    clock_t startTime, endTime;
    startTime = clock();

    cout << "CBC Mode:" << endl;

    if(argc < 3){
        cout << "参数错误=_=" << endl;
        return 0;
    }else if (argc >3){
        ofile = argv[3];
    }else{
        ;
    }
    ifile = argv[2];

    if(argv[1][0] == 'e'){
        cout << "Buffer Size: " << BufferSIZE << 'B' << endl;
        if(argc <= 3){
            ofile = ifile + ".aeons";
        }
        encrypt(ifile, ofile);
        cout << "文件保存在: " << ofile << endl;
    }else if(argv[1][0] == 'd'){
        cout << "Buffer Size: " << BufferSIZE << 'B' << endl;
        if(argc <= 3){
            ofile = ifile.substr(0,ifile.find_last_of('.'));
        }
        decrypt(ifile, ofile);
        cout << "文件保存在: " << ofile << endl;
    }else {
        cout << "参数错误=_=" << endl;
        return 0;
    }


    endTime = clock();
    cout << "运行时长: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
