#include "TextModeScreen.h"

char hexToStringOutput[128];
char integerToStringOutput[128];

void clear_screen(uint8_t color = 0x00)
{
    long *ptr = (long*)0xb8000;
    long ptr_val = 0;
    ptr_val |= ((uint64_t)color) << 56;
    ptr_val |= ((uint64_t)0x20) << 48;
    ptr_val |= ((uint64_t)color) << 40;
    ptr_val |= ((uint64_t)0x20) << 32;
    ptr_val |= ((uint64_t)color) << 24;
    ptr_val |= ((uint64_t)0x20) << 16;
    ptr_val |= ((uint64_t)color) << 8;
    ptr_val |= (uint64_t)0x20;
    

    for(int i = 0; i < 500; i++)
    {
        *ptr = ptr_val; 
        ptr++;
    }

    set_cursor(0, 0);
}

void set_cursor_position(uint16_t pos)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void set_cursor(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;
    set_cursor_position(pos);
}

void print_string_color(uint8_t color, const int8_t *data)
{

    uint16_t *screen = (uint16_t*)0xb8000;
    screen += get_cursor_position();
    
    while(data[0] != 0)
    {
        if(*data == '\n')
        {
            screen += VGA_WIDTH;
            screen = (((screen - (uint16_t*)0xb8000) / VGA_WIDTH) * VGA_WIDTH) + (uint16_t*)0xb8000;
        }
        else if(*data == '\r')
        {
            screen = (((screen - (uint16_t*)0xb8000) / VGA_WIDTH) * VGA_WIDTH) + (uint16_t*)0xb8000;
        }
        else
        {
            *screen = (((uint16_t)color) << 8) + *data;
            screen++;
        }
        data++;
    }

    uint16_t cursor_pos = screen - (uint16_t*)0xb8000;
    set_cursor(cursor_pos % VGA_WIDTH, cursor_pos / VGA_WIDTH);
}

void print_string(const int8_t *data)
{
    print_string_color(0x0f, data);
}

void print_char(uint8_t data, uint8_t color)
{
    uint16_t *screen = (uint16_t*)0xb8000;
    screen += get_cursor_position();
    *screen = ((uint16_t)color << 8) + data;
    uint16_t screen_pos = (uint16_t)(screen - (uint16_t*)0xb8000);
    screen_pos += 1;
    set_cursor(screen_pos % VGA_WIDTH, screen_pos / VGA_WIDTH);
}

uint16_t get_cursor_position()
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

char floatToStringOutput[128];
const char* FloatToString(float value, uint8_t decimal_places)
{
    char* intPtr = (char*)IntegerToString((int)value);
    char* floatPtr = floatToStringOutput;

    if(value < 0)
    {
        value *= -1;
    }

    while(*intPtr != 0)
    {
        *floatPtr++ = *intPtr++;
    }

    *floatPtr++ = '.';

    float newValue = (value - (int)value);

    for(uint8_t i = 0; i < decimal_places; i++)
    {
        newValue *= 10;
        *floatPtr++ = (int)newValue + 48;
        newValue = newValue - (int)newValue;
    }

    *floatPtr = 0;

    return floatToStringOutput;
}