/*
* Author: Aeonni
* Time: 2019-06-07
*/
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <string>

#include "libs/base64.h"
#include "libs/cmdline.h"
#include "lib_json/json/json.h"
#include "rsa.h"
#include "md5.h"

using namespace std;

int main(int argc, char * argv[]){
    // 命令行参数设置
    cmdline::parser config;

    config.add("sign", 's', "（任务）进行签名");
    // config.add("verify", 'v', "（任务）进行验证");
    config.add<string>("verify", 'v', "（任务）进行验证", false, "");
    config.add("rsa-test", '\0', "（任务）RSA 加密功能测试");

    config.add<string>("file", 'f', "输入一个文件路径", false, "");
    config.add<string>("string", '\0', "输入一个字符串", false, "");
    config.add<string>("key-file", 'k', "输入一个密钥文件路径", false, "");

    config.add<int>("bits", 'b', "RSA 模数", false, 2048, cmdline::oneof<int>(256, 512, 1024, 2048, 4096));

    config.add("help", 0, "Print this message");
    config.set_program_name("aeonni.sig");
    config.parse_check(argc, argv);

    // MD5 related varibles
    md5_state_t md5_state;
    md5_byte_t digest[16];
    char buf[16][2];

    // 功能选择
    if(config.exist("rsa-test") && !(config.exist("sign") || config.exist("verify"))) {
        // RSA 加密功能测试
        test(config.get<int>("bits"));
    }else if((config.exist("sign") ^ config.exist("verify")) && !config.exist("rsa-test")) {
        // 计算 MD5
        if (config.get<string>("string").length() > 0) {
            cout << "正在计算 MD5 hash..." << endl;
            md5_init(&md5_state);
            md5_append( &md5_state, 
                        (const md5_byte_t*)config.get<string>("string").data(), 
                        config.get<string>("string").length() );
            md5_finish(&md5_state, digest);
        } else if (config.get<string>("file").length() > 0) {

            // Read file
            ifstream rfile;
            rfile.open(config.get<string>("file"), ios::in | ios::binary);
            long fsize = rfile.tellg();
            if(fsize < 0) {
                cout << "无法读取文件!" << endl;
                return -2;
            }

            rfile.seekg (0, rfile.end);
            long length = rfile.tellg();
            rfile.seekg (0, rfile.beg);

            cout << "Reading file... size = " << length << "B" << endl;

            char * buffer = new char [length];

            rfile.read(buffer, length);

            cout << "正在计算 MD5 hash..." << endl;

            md5_init(&md5_state);
            md5_append( &md5_state,
                        (const md5_byte_t*)buffer,
                        length );
            md5_finish(&md5_state, digest);
        } else {
            cout << "没有指定的文件名或字符串！！\n" << endl;
            cout << config.usage();
            return -1;
        }

        cout << "\033[36mMD5 : \033[0m";

        for(int i = 0; i < 16; i++){
            sprintf(buf[i], "%.2x", digest[i]);
        } string md5 = (char*)buf;
        cout << md5 << endl;

        // MD5 计算完毕

        // RSA related varibles
        mpz_t p; mpz_init(p);
        mpz_t q; mpz_init(q);
        mpz_t n; mpz_init(n);
        mpz_t e; mpz_init(e);
        mpz_t d; mpz_init(d);
        string s ;
        size_t strsize;

        if(config.exist("sign")) {
            // 进行签名
            cout << "进行签名任务..." << endl;
            cout << "查询是否有可用的私钥...（rsa.private）" << endl;

            ifstream rfile;
            string filename = config.get<string>("key-file").length() > 0 ? config.get<string>("key-file") : "rsa.private";
            rfile.open(filename, ios::in | ios::binary);
            long fsize = rfile.tellg();
            if(fsize < 0) {
                cout << "找不到可用的私钥文件! 准备生成..." << endl;
                gen_keys(p, q, n, e, d, config.get<int>("bits"));
                s = base64_encode(to_string(e, &strsize), strsize);
                cout << "\033[36m加密公钥 (Base64):\033[0m" << endl << s << endl;
                s = base64_encode(to_string(d, &strsize), strsize);
                cout << "\033[36m解密私钥 (Base64):\033[0m" << endl << s << endl;

                Json::Value root_private;
                root_private["d"] = base64_encode(to_string(d, &strsize), strsize);
                root_private["d_size"] = (int)strsize;
                root_private["n"] = base64_encode(to_string(n, &strsize), strsize);
                root_private["n_size"] = (int)strsize;
                ofstream prif;
                prif.open ("rsa.private", ios::out | ios::trunc | ios::binary );
                prif << root_private.toStyledString();
                prif.close();

                Json::Value root_public;
                root_public["e"] = base64_encode(to_string(e, &strsize), strsize);
                root_public["e_size"] = (int)strsize;
                root_public["n"] = base64_encode(to_string(n, &strsize), strsize);
                root_public["n_size"] = (int)strsize;
                ofstream pubf;
                pubf.open ("rsa.public", ios::out | ios::trunc | ios::binary );
                pubf << root_public.toStyledString();
                pubf.close();

                cout << "\033[36m密钥文件已保存！ \033[0m" << endl;
            } else {
                Json::Reader reader;// 解析json用Json::Reader
                Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array
                if (reader.parse(rfile, root, false)) {
                    string ds = root["d"].asString();
                    string ns = root["n"].asString();
                    mpz_import(d, root["d_size"].asInt(), 1, sizeof(char), 0, 0, base64_decode(ds.data(), ds.length()));
                    mpz_import(n, root["n_size"].asInt(), 1, sizeof(char), 0, 0, base64_decode(ns.data(), ns.length()));
                }
                rfile.close();
            }
            mpz_t c; mpz_init(c);
            mpz_t m; mpz_init(m);
            from_string(m, md5);
            mpz_powm(c, m, d, n);
            s = base64_encode(to_string(c, &strsize), strsize);
            cout << "\033[36m签名 (Base64):\033[0m" << endl << s << endl;
        }else {
            // 进行验证
            cout << "进行签名验证..." << endl;

            string fname = "rsa.public";
            if(config.get<string>("key-file").length() > 0) {
                fname = config.get<string>("key-file");
            } else {
                cout << "查询是否有可用公钥...（rsa.public）" << endl;
            }

            ifstream rfile;
            rfile.open(fname, ios::in | ios::binary);
            long fsize = rfile.tellg();
            int imp_size;
            if(fsize < 0) {
                cout << "找不到可用的私钥文件! (" << fname <<')'<< endl;
                cout << "exit" << endl;
                return -1;
            } else {
                Json::Reader reader;// 解析json用Json::Reader
                Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array
                if (reader.parse(rfile, root, false)) {
                    string es = root["e"].asString();
                    string ns = root["n"].asString();
                    mpz_import(e, root["e_size"].asInt(), 1, sizeof(char), 0, 0, base64_decode(es.data(), es.length()));
                    mpz_import(n, root["n_size"].asInt(), 1, sizeof(char), 0, 0, base64_decode(ns.data(), ns.length()));
                    imp_size = root["n_size"].asInt();
                }
                rfile.close();
            }
            mpz_t c; mpz_init(c);
            mpz_t m; mpz_init(m);

            mpz_import(m, imp_size, 1, sizeof(char), 0, 0, base64_decode(config.get<string>("verify").data(), config.get<string>("verify").length()));

            mpz_powm(c, m, e, n);
            s = to_string(c, &strsize);
            cout << "\033[36m校验结果:\033[0m" << s << endl;

            if(s == md5) {
                cout << "\033[36m签名验证成功！\033[0m" << endl;
            }else {
                cout << "\033[31m签名验证失败！\033[0m" << endl;
            }

        }
    } else {
        cout << "没有输入有意义的任务参数！！！" << endl;
    }


    return 0;
}
