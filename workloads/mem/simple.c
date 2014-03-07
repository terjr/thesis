#include <stdlib.h>
#include <stdio.h>

#define SIZE (1024)

int main()
{
    long i;
    char* ptr = malloc(sizeof(char)*SIZE);
    for (i = 0; i < SIZE; ++i)
    {
        ptr[i] = 0;
        printf("\r%ld", i);
    }
    return 0;
}
