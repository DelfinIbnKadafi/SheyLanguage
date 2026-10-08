#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "sheylang.h"

int error(const char *message, int line) {
    printf("Error (%d): %s\n", line, message);
    return 0;
}

int parser(const char *keyword, const char *buffer, int line) {
    // Proses parsing untuk keyword "tampilkan"
    if(strcmp(keyword, "tampilkan") == 0) {
        char argument[1024];
        int consumed = 0;
        
        // cek apakah sintaks untuk keyword "tampilkan" valid
        // %n dipakai untuk memastikan tanda kutip penutup ada, karena sscanf tetap return 1 walaupun tanda kutip penutupnya tidak ada
        if(sscanf(buffer, "tampilkan \"%1023[^\"]\"%n", argument, &consumed) != 1 || consumed == 0) {
            // ambil argument tanpa tanda kutip, karena sscanf di atas tidak mengisi argument jika gagal
            if(sscanf(buffer, "tampilkan %1023s", argument) != 1) {
                error("Argument tidak valid", line);
                return 1;
            }

            // cek apakah argument adalah data
            // cek jika data adalah variabel (1)
            if(sheylangCekData(argument) == 1) {
                if(strcmp(sheylangAmbilTipe(argument), "int") == 0) {
                    int intValue = sheylangAmbilInteger(argument);
                    sprintf(argument, "%d", intValue);
                }
                else if(strcmp(sheylangAmbilTipe(argument), "float") == 0) {
                    float floatValue = sheylangAmbilFloat(argument);
                    sprintf(argument, "%f", floatValue);
                }
                else if(strcmp(sheylangAmbilTipe(argument), "string") == 0) {
                    strcpy(argument, sheylangAmbilString(argument));
                }
            }
            else {
                // argument bukan string dan bukan variabel yang dikenal
                error("Variabel tidak ditemukan", line);
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
                    // simpan hasilnya dulu supaya sheylangVarBaru tidak dipanggil dua kali
                    int hasil = sheylangVarBaru(varName, varType);
                    if(hasil == 1) {
                        error("Variabel dengan nama tersebut sudah digunakan", line);
                        return 1;
                    }
                    else if(hasil == 2) {
                        error("Jumlah variabel sudah mencapai batas", line);
                        return 1;
                    }
                    return 0;
                }
                else {
                    error("Argument tidak valid", line);
                    return 1;
                }
            }
            else {
                error("Tipe data tidak valid", line);
                return 1;
            }
        }
        // jika keyword var tidak diikuti apa pun, maka tipe data belum diberikan
        else {
            error("Argument tidak valid", line);
            return 1;
        }
    }

    // Proses parsing untuk keyword ubah yang berguna untuk mengubah nilai variabel
    else if(strcmp(keyword, "ubah") == 0) {
        char varName[50];

        // cek apakah keyword ubah diikuti dengan nama variabelnya
        if(sscanf(buffer, "ubah %49s", varName) == 1) {
            // cek apakah variabelnya ada, karena sheylangAmbilTipe mengembalikan NULL jika tidak ketemu dan strcmp akan crash
            if(sheylangCekData(varName) == 0) {
                error("Nama variabel tidak diketahui", line);
                return 1;
            }

            if(strcmp(sheylangAmbilTipe(varName), "int") == 0) {
                int nilaibaru;
                if(sscanf(buffer, "ubah %49s %d", varName, &nilaibaru) == 2) {
                    if(sheylangUbahVar(varName, &nilaibaru) == 0) {
                        return 0;
                    }
                    error("Nama variabel tidak diketahui", line);
                    return 1;
                }
                else {
                    error("Argument tidak valid", line);
                    return 1;
                }
            }
            else if(strcmp(sheylangAmbilTipe(varName), "float") == 0) {
                float nilaibaru;
                if(sscanf(buffer, "ubah %49s %f", varName, &nilaibaru) == 2) {
                    if(sheylangUbahVar(varName, &nilaibaru) == 0) {
                        return 0;
                    }
                    error("Nama variabel tidak diketahui", line);
                    return 1;
                }
                else {
                    error("Argument tidak valid", line);
                    return 1;
                }
            }
            else if(strcmp(sheylangAmbilTipe(varName), "string") == 0) {
                char nilaibaru[1024];
                if(sscanf(buffer, "ubah %49s %1023s", varName, nilaibaru) == 2) {
                    if(sheylangUbahVar(varName, nilaibaru) == 0) {
                        return 0;
                    }
                    error("Nama variabel tidak diketahui", line);
                    return 1;
                }
                else {
                    error("Argument tidak valid", line);
                    return 1;
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