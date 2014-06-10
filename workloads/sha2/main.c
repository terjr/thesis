#include "sha2.h"
#include "sha2hash.h"

#include <stdio.h>

#define BUFLEN 64

int main() {
    int kl, l, fd;
    SHA512_CTX ctx;
    unsigned char buf[BUFLEN];

    SHA512_Init(&ctx);
    SHA512_Update(&ctx, (unsigned char*) testhash_txt, testhash_txt_len);

    SHA512_End(&ctx, buf);
    printf("SHA512:\n%s\n", buf);

    return 0;
}
