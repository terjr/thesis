#define SIZE (512*1024)


int main()
{
    char ptr[SIZE];
    volatile long i;
    volatile long sum = 0;
    for (i = 0; i < SIZE; ++i)
    {
        sum -= i;
    }
    for (i = 0; i < SIZE; ++i)
    {
        sum *= i;
    }
    return 0;
}
