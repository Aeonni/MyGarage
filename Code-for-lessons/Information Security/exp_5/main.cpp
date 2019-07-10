#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "md5.h"
#include "cmdline.h"

using namespace std;

int main(int argc, char **argv) {

    // Parse arguments
    cmdline::parser config;
    config.add("upper", 'u', "输出大写 MD5 序列（默认小写）");
    config.add<string>("file", 'f', "输入一个文件路径", false, "");
    config.add<string>("string", 's', "输入一个字符串", false, "");
    config.add("help", 'h', "Print this message");
    config.set_program_name("aeonni.hash.md5");
    config.parse_check(argc, argv);



    md5_state_t md5_state;
    md5_byte_t digest[16];
    char buf[16][3];

    if (config.get<string>("string").length() > 0) {
        cout << "Calculating MD5 hash..." << endl;
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

        cout << "Calculating MD5 hash..." << endl;

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
 

    // Print results.
    cout << "\nMD5 : ";
    for(int i = 0; i < 16; i++){
        sprintf(buf[i], config.exist("upper") ? "%.2X" : "%.2x", digest[i]);
        cout << buf[i];
    } cout << endl;
    
    return 0;
} 