#include <stdio.h>
#include <string.h>
#include "sheylang.h"
#include "parser.h"
#include "lexer.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Cek apakah ada argumen input file
    if(argc < 2) {
        printf("Tidak ada input file\n");
        return 1;
    }

    const char *extension = strrchr(argv[1], '.');
    if(extension == NULL || strcmp(extension, ".shy") != 0) {
        printf("Hanya dapat membuka file dengan extensi .shy\n");
        return 1;
    }

    // oper ke lexer untuk membaca file dan mengubahnya menjadi token
    lexer(argv[1]);
    return 0;
}