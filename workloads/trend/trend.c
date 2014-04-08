#define SIZE (256*1024)


int main()
{
    char ptr[SIZE];
    volatile long i;
    for (i = 0; i < SIZE; ++i)
    {
        ptr[i] = 0;
    }
    volatile long sum = 0;
    for (i = 0; i < SIZE; ++i)
    {
        sum += i;       
    }
    return 0;
}
