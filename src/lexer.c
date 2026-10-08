#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int lexer(const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if(fptr == NULL) {
        printf("Tidak dapat menemukan file\n");
        return 1;
    }

    char buffer[1024];
    int line = 0;

    while(fgets(buffer, sizeof(buffer), fptr) != NULL) {
        line++;
        
        // cek apakah baris kosong
        if(strlen(buffer) == 0 || buffer[0] == '\n') {
            continue;
        }

        // ambil kata pertama sebagai keyword
        char keyword[50];
        // %49s supaya tidak melebihi ukuran keyword, baris yang isinya cuma spasi atau enter (misal \r\n dari file windows) dilewati
        if(sscanf(buffer, "%49s", keyword) != 1) {
            continue;
        }
        // biarkan parser mengecek syntax dan mengirim sinyal ke vm
        // jika parser mendapati error, maka parser akan mengirimkan sinyal return selain 0
        // dan parser akan print error, lalu lexer akan break
        if(parser(keyword, buffer, line) != 0) {
            break;
        }
        continue;
    }

    fclose(fptr);
    return 0;
}