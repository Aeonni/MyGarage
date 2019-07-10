#include "encoding.h"

// 保存为文件
void to_file(const char * filename, mpz_t data) {
    FILE *fp;
    fp = fopen(filename,"w");
    if(fp) puts("文件保存成功");
    else puts("文件保存成败");
    mpz_out_raw(fp, data);
    fclose(fp);
}

void from_string(mpz_t data, string str) {
    mpz_import(data, str.length(), 1, sizeof(char), 0, 0, str.data());
}

char * to_string(mpz_t data, size_t * s) {
    size_t size = mpz_sizeinbase(data, 2);
    size = (size % 8 == 0) ? size/8+1 : size/8+2;
    char * p = (char*)malloc(size);
    size -= 1;
    p[size] = '\0';
    if (s != NULL) *s = size;
    return (char*)mpz_export(p, &size, 1, sizeof(char), 0, 0, data);
}