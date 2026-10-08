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

// jumlah variabel yang sudah terpakai, jadi index yang valid hanya 0 sampai varCount - 1
int varCount = 0;

// fungsi di vm yang menjalankan perintah tampilkan
int sheylangTampilkan(const char *message) {
    printf("%s\n", message);
    return 0;
}

// fungsi untuk mendapatkan tipe data
char* sheylangAmbilTipe(const char *dataname) {
    // coba cek tipe data variabel
    for(int i = 0; i < varCount; i++) {
        if(strcmp(dataname, var[i].name) == 0) {
            return var[i].type;
        }
    }
    return NULL;
}

// fungsi yang mengambil informasi data
int sheylangCekData(const char *dataname) {
    // coba lihat apakah data adalah variabel
    for(int i = 0; i < varCount; i++) {
        if(strcmp(dataname, var[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

// ambil value
char* sheylangAmbilString(const char *dataname) {
    // coba cek apakah data adalah variabel dan ambil nilainya
    for(int i = 0; i < varCount; i++) {
        if(strcmp(dataname, var[i].name) == 0) {
            return var[i].value.stringValue;
        }
    }
    return NULL;
}

int sheylangAmbilInteger(const char *dataname) {
    // coba cek apakah data adalah variabel dan ambil nilainya
    for(int i = 0; i < varCount; i++) {
        if(strcmp(dataname, var[i].name) == 0) {
            return var[i].value.intValue;
        }
    }
    return 1;
}

float sheylangAmbilFloat(const char *dataname) {
    // coba cek apakah data adalah variabel dan ambil nilainya
    for(int i = 0; i < varCount; i++) {
        if(strcmp(dataname, var[i].name) == 0) {
            return var[i].value.floatValue;
        }
    }
    return 1;
}

// fungsi menambahkan variabel baru
int sheylangVarBaru(const char *varname, const char *type) {
    // cek jika nama variabel sudah digunakan
    for(int i = 0; i < varCount; i++) {
        if(strcmp(varname, var[i].name) == 0) {
            return 1;
        }
    }

    // cek jika tempat penyimpanan variabel sudah penuh, return 2 supaya parser bisa membedakan dengan nama yang sudah dipakai
    if(varCount >= (int)(sizeof(var) / sizeof(var[0]))) {
        return 2;
    }

    // simpan variabel baru di slot kosong, snprintf dipakai supaya nama dan tipe tidak overflow
    snprintf(var[varCount].name, sizeof(var[varCount].name), "%s", varname);
    snprintf(var[varCount].type, sizeof(var[varCount].type), "%s", type);
    varCount++;
    return 0;
}

// fungsi mengubah nilai variabel
int sheylangUbahVar(const char *varname, void *nilaibaru) {
    for(int i = 0; i < varCount; i++) {
        if(strcmp(varname, var[i].name) == 0) {
            if(strcmp(var[i].type, "int") == 0) {
                var[i].value.intValue = *(int *)nilaibaru;
                return 0;
            }
            else if(strcmp(var[i].type, "float") == 0) {
                var[i].value.floatValue = *(float *)nilaibaru;
                return 0;
            }
            else if(strcmp(var[i].type, "string") == 0) {
                // string dipotong jika lebih dari 255 karakter supaya tidak overflow
                snprintf(var[i].value.stringValue, sizeof(var[i].value.stringValue), "%s", (char *)nilaibaru);
                return 0;
            }
        }
    }
    return 1;
}