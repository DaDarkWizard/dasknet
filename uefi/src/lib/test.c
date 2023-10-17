#include "stdarg.h"
//#include <stdio.h>

void test(volatile int n, ...);

int main()
{
    test(1, 3);
}

void test(volatile int n, ...)
{
    va_list x;

    

    va_start(x, n);
    printf("Test: %ld\n", &n);
    printf("Previous value: %d\n", *(&n - 4 * sizeof(void*)));
    printf("Previous value: %d\n", *(&n - 3 * sizeof(void*)));
    printf("Previous value: %d\n", *(&n - 2 * sizeof(void*)));
    printf("Previous value: %d\n", *(&n - sizeof(void*)));
    printf("Test value: %d\n", *(&n));
    printf("Next value: %d\n", *(&n + sizeof(void*)));
    printf("Next value: %d\n", *(&n + 2 * sizeof(void*)));
    printf("Next value: %d\n", *(&n + 3 * sizeof(void*)));
    printf("Next value: %d\n", *(&n + 4 * sizeof(void*)));
    printf("Next value: %d\n", *(&n + 5 * sizeof(void*)));
    printf("Next value: %d\n", *(&n + 6 * sizeof(void*)));
    printf("Next value: %d\n", *(&n + 7 * sizeof(void*)));
    printf("Number: %d\n", va_arg(x, int));
    va_end(x);
}