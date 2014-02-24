#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libscrypt.h>

#define BUFLEN 4096

int main(int argc, char **args) {
    uint8_t *outbuf = malloc(sizeof(uint8_t) * BUFLEN);
    int retcode = 0;

    retcode = libscrypt_scrypt((uint8_t*)"passord", strlen("passord"), (uint8_t*)"sahjt", strlen("sahjt"), SCRYPT_N*4, SCRYPT_r, SCRYPT_p, outbuf, BUFLEN);

    return retcode;
}
