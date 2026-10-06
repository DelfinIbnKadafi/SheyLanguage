#include <stdio.h>
#include <string.h>
#include "sheylang.h"

// fungsi di vm yang menjalankan perintah tampilkan
int sheylangTampilkan(const char *message) {
    printf("%s\n", message);
    return 0;
}