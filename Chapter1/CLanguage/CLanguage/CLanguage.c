#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    int n;

    n = add(1, 2);
    printf("%d\n", n);
    return 0;
}