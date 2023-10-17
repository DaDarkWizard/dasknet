#include "string.h"

size_t strlen(const char* str)
{
    size_t i = 0;
    for(i = 0; str[i] != NULL; i++);
    return --i;
}