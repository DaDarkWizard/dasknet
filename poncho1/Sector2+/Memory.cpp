#include "Memory.h"

void memset(void *start, uint64_t value, uint64_t num)
{
    if(num <= 8)
    {
        uint8_t* valPtr = (uint8_t*)&value;
        for(uint8_t* ptr = (uint8_t*)start; ptr < (uint8_t*)((uint64_t)start + num); ptr++)
        {
            *ptr = *valPtr;
            valPtr++;
        }

        return;
    }

    uint64_t proceedingBytes = num % 8;
    uint64_t newnum = num - proceedingBytes;

    for(uint64_t* ptr = (uint64_t*)start; ptr < (uint64_t*)((uint64_t)start + num); ptr++)
    {
        *ptr = value;
    }

    uint8_t* valPtr = (uint8_t*)&value;
    for(uint8_t* ptr = (uint8_t*)((uint64_t)start + newnum); ptr < (uint8_t*)((uint64_t)start + newnum); ptr++)
    {
        *ptr = *valPtr;
        valPtr++;
    }
}

void memcpy(void *dest, void *src, uint64_t num)
{
    if(num <= 8)
    {
        uint8_t* valPtr = (uint8_t*)src;
        for(uint8_t* ptr = (uint8_t*)dest; ptr < (uint8_t*)((uint64_t)dest + num); ptr++)
        {
            *ptr = *valPtr;
            valPtr++;
        }

        return;
    }

    uint64_t proceedingBytes = num % 8;
    uint64_t newnum = num - proceedingBytes;
    uint64_t *srcptr = (uint64_t*)src;

    for(uint64_t* destptr = (uint64_t*)dest; destptr < (uint64_t*)((uint64_t)dest + newnum); destptr++)
    {
        *destptr = *srcptr;
        srcptr++;
    }

    uint8_t* srcptr8 = (uint8_t*)((uint64_t)src + newnum);
    for(uint8_t* destptr8 = (uint8_t*)((uint64_t)dest + newnum); destptr8 < (uint8_t*)((uint64_t)dest + num); destptr8++)
    {
        *destptr8 = *srcptr8;
        srcptr8++;
    }
}