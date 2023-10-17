#include "stdlib.h"

int abs(int x)
{
    return x < 0 ? -1 * x : x;
}

int atoi(const char* str)
{
    int out = 0;
    int dec = 1;
    int i = 0;
    int negative = str[0] == '-';
    if(negative) { str = str + __SIZEOF_POINTER__; }
    if(!(str[i] <= '9' && str[i] >= '0'))
        return 0;
    for(; str[i] <= '9' && str[i] >= '0'; i++);
    i--;
    for(; i >= 0; i--)
    {
        out += dec * (str[i] - '0');
        dec *= 10;
    }
    return out;
}