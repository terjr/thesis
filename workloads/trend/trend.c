#include <stdio.h>
#include <stdlib.h>

#define SIZE (512*1024)

int main()
{
    volatile long i;
    char* ptr = malloc(sizeof(char)*SIZE);
    for (i = 0; i < SIZE; ++i)
    {
        ptr[i] = 0;
    }
    volatile long sum = 0;
    for (i = 0; i < SIZE; ++i)
    {
        sum += i;       
    }
    printf("%ld, %ld \n", sum, ptr[i-18]);
    return 0;
}
