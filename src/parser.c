#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
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

int error(const char *message, int line) {
    printf("Error (%d): %s\n", line, message);
    return 0;
}

int parser(const char *keyword, const char *buffer, int line) {
    // Proses parsing untuk keyword "tampilkan"
    if(strcmp(keyword, "tampilkan") == 0) {
        char argument[1024];
        
        // cek apakah sintaks untuk keyword "tampilkan" valid
        if(sscanf(buffer, "tampilkan \"%1023[^\"]\"", argument) != 1) {
            // cek jika yang ingin ditampilkan adalah variabel
            if(sscanf(buffer, "tampilkan %49s", argument) == 1) {
                bool ketemu = false;
                for(int i = 0; i <= varCount; i++) {
                    // jika namanya sama dengan variabel
                    if(strcmp(argument, var[i].name) == 0) {
                        // ambil nilai dan masukkan kedalam argument sesuai tipe
                        ketemu = true;
                        if(strcmp(var[i].type, "int") == 0) {
                            sprintf(argument, "%d", var[i].value.intValue);
                        }
                        else if(strcmp(var[i].type, "float") == 0) {
                            sprintf(argument, "%f", var[i].value.floatValue);
                        }
                        else if(strcmp(var[i].type, "string") == 0) {
                            sprintf(argument, "%s", var[i].value.stringValue);
                        }
                    }
                }
                if(!ketemu) {
                    error("Tidak ditemukan data tersebut!", line);
                    return 1;
                }
            }
            else {
                error("Sintaks tidak valid untuk keyword 'tampilkan'", line);
                return 1;
            }
            
        }

        // panggil fungsi di vm untuk menjalankan perintah tampilkan
        sheylangTampilkan(argument);

        // berikan sinyal bahwa parsing berhasil
        return 0;
    }

    // proses keyword keluar
    else if(strcmp(keyword, "keluar") == 0) {
        // wajibkan tidak ada argumen tambahan
        char tmp[512];
        if(sscanf(buffer, "keluar %s", tmp) == 1) {
            error("Fungsi keluar tidak boleh memiliki argumen tambahan!", line);
            return 1;
        }
        else {
            // kirim return 1 kepada lexer agar berhenti membaca file
            return 1;
        }
    }

    // Proses parsing untuk variabel
    else if(strcmp(keyword, "var") == 0) {
        char varType[10], varName[50];

        // cek apakah keyword var diikuti dengan tipe data yang benar
        if(sscanf(buffer, "var %9s", varType) == 1) {
            if(strcmp(varType, "int") == 0 || strcmp(varType, "float") == 0 || strcmp(varType, "string") == 0) {
                // cek apakah keyword var dan tipe data diikuti dengan nama variabelnya
                if(sscanf(buffer, "var %9s %49s", varType, varName) == 2) {

                    // cek apakah nama variabel sudah pernah digunakan
                    for(int i = 0; i <= varCount; i++) {
                        if(strcmp(var[i].name, varName) == 0) {
                            error("Nama variabel sudah digunakan oleh variabel lain", line);
                            return 1;
                        }
                    }

                    varCount++;

                    // tambahkan variabel baru, tipe dan namanya
                    strcpy(var[varCount].type, varType);
                    strcpy(var[varCount].name, varName);
                }
                // jika tidak ada nama variabel
                else {
                    error("Nama variabel tidak ditemukan", line);
                    return 1;
                }
            }
            else {
                error("Tipe data tidak valid", line);
                return 1;
            }
        }
    }

    // Proses parsing untuk keyword ubah yang berguna untuk mengubah nilai variabel
    else if(strcmp(keyword, "ubah") == 0) {
        char varName[50];
        char varType[10];
        int varid;

        // cek apakah keyword ubah diikuti dengan nama variabelnya
        if(sscanf(buffer, "ubah %49s", varName) == 1) {
            // cek apakah nama variabel ada
            bool ketemu = false;
            for(int i = 0; i <= varCount; i++) {
                // jika ada nama variabel tersebut
                if(strcmp(var[i].name, varName) == 0) {
                    strcpy(varType, var[i].type);
                    varid = i;
                    ketemu = true;
                    break;
                }
            }
            // jika tidak ketemu
            if(!ketemu) {
                error("Tidak ditemukan variabel tersebut!", line);
                return 1;
            }
            // jika ketemu
            else {
                // ambil nilai baru variabel sesuai tipe data
                // integer
                if(strcmp(varType, "int") == 0) {
                    int nilaibaru;
                    if(sscanf(buffer, "ubah %49s %d", varName, &nilaibaru) == 2) {
                        var[varid].value.intValue = nilaibaru;
                    }
                    else {
                        error("Argumen tidak valid!", line);
                        return 1;
                    }
                }
                // float
                else if(strcmp(varType, "float") == 0) {
                    float nilaibaru;
                    if(sscanf(buffer, "ubah %49s %f", varName, &nilaibaru) == 2) {
                        var[varid].value.floatValue = nilaibaru;
                    }
                    else {
                        error("Argumen tidak valid!", line);
                        return 1;
                    }
                }
                // string
                else if(strcmp(varType, "string") == 0) {
                    char nilaibaru[256];
                    if(sscanf(buffer, "ubah %49s \"%255[^\"]\"", varName, nilaibaru) == 2) {
                        strcpy(var[varid].value.stringValue, nilaibaru);
                    }
                    else {
                        error("Argumen tidak valid!", line);
                        return 1;
                    }
                }
            }
        }
        else {
            error("Argumen tidak valid!", line);
            return 1;
        }
    }

    // tidak menemukan 1 pun keyword yang dikenali
    else {
        error("Fungsi tidak dikenali", line);
        return 1;
    }
    return 0;
}