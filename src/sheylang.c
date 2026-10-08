#include <stdio.h>
#include <string.h>
#include "sheylang.h"

// data yang menyimpan vaiabel, id variabel, nama variabel, dan value variabel sesuai dengan tipe data yang diinginkan
typedef struct {
    char name[50];
    char type[10];
    union {
        int intValue;
        float floatValue;
        char stringValue[256];
    } value;
} Variable;

Variable var[256];

int varCount = 0;

// fungsi di vm yang menjalankan perintah tampilkan
int sheylangTampilkan(const char *message) {
    printf("%s\n", message);
    return 0;
}

// fungsi untuk mendapatkan tipe data
char* sheylangAmbilTipe(const char *dataname) {
}