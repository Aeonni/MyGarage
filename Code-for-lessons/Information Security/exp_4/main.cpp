/*
* Author: Aeonni
* Time: 2019-04-28
*/
#include <iostream>  
#include <time.h>
#include <fstream>
#include <string>  

#include "libs/base64.h"
#include "libs/cmdline.h"
#include "rsa.h"
using namespace std;


int main(int argc, char * argv[]){
    cmdline::parser config;
    int did = 0;

    config.add("do-test", '\0', "加密功能测试");
    config.add<int>("bits", '\0', "模数", false, 2048, cmdline::oneof<int>(256, 512, 1024, 2048, 4096));
    // config.add<string>("out-dir", 'o', "protocol type", false, "http", cmdline::oneof<string>("http", "https", "ssh", "ftp"));
    // config.add("gzip", '\0', "gzip when transfer");
    config.add("help", 0, "Print this message");
    config.set_program_name("aeonni.rsa");

    config.parse_check(argc, argv);


    if(config.exist("do-test")) {
        test(config.get<int>("bits"));
        did++;
    }
    

    if(!did) {cout << "Did nothing..." << endl;}
    // to_file("c.data", c);

}



