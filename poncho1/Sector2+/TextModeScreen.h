#pragma once

#include "types.h"
#include "IO.h"

#ifndef VGA_WIDTH
#define VGA_WIDTH 80
#endif

uint16_t get_cursor_position();
void clear_screen(uint8_t color);
void set_cursor(int x, int y);
void print_string_color(uint8_t color, const int8_t *data);
void print_string(const int8_t *data);
void print_char(uint8_t data, uint8_t color = 0x0f);
void set_cursor_position(uint16_t position);

extern char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value)
{
    T *valPtr = &value;
    uint8_t *ptr;
    uint8_t temp;
    uint8_t size = (sizeof(T)) * 2 - 1;
    uint8_t i;
    for(i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        temp = ((*ptr & 0xF0) >> 4);
        hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
        temp = ((*ptr & 0x0F));
        hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
    }
    hexToStringOutput[size + 1] = 0;
    return hexToStringOutput;
}

extern char integerToStringOutput[128];
template<typename T>
const char* IntegerToString(T value)
{
    uint8_t size = 0;

    uint8_t is_negative = 0;

    if(value < 0)
    {
        is_negative = 1;
        value *= -1;
        integerToStringOutput[0] = '-';
    }

    uint64_t size_tester = (uint64_t)value;
    while(size_tester / 10 > 0)
    {
        size_tester /= 10;
        size++;
    }

    uint8_t index = 0;
    uint64_t newValue = (uint64_t)value;
    while(newValue / 10 > 0)
    {
        uint8_t remainder = newValue % 10;
        newValue /= 10;
        integerToStringOutput[is_negative + size - index] = remainder + 48;
        index++;
    }
    uint8_t remainder = newValue % 10;
    integerToStringOutput[is_negative + size - index] = remainder + 48;
    integerToStringOutput[is_negative + size + 1] = 0;
    return integerToStringOutput;
}

const char* FloatToString(float value, uint8_t decimal_places);