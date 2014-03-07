#include <stdio.h>
#include <stdlib.h>

#define N 8192

unsigned long long *fibs;

int main()
{
    int i;
    fibs = malloc(sizeof(*fibs)*N);
    fibs[0] = 1;
    fibs[1] = 1;
    for (i = 2; i < N; ++i)
    {
        fibs[i] = fibs[i-1] + fibs[i-2];
    }

    i = rand()%N;
    printf("Fib. %d is %d\n", i, fibs[i]);
    return 0;
}
