#include "screen.h"
#include "../kernel/low_level.h"

int handle_scrolling(int offset);
int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);

void print_char(char character, int col, int row, char attribute_bytes)
{
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;

    if(!attribute_bytes)
    {
        attribute_bytes = GREEN_ON_BLACK;
    }

    int offset;

    if(col >= 0 && row >= 0)
    {
        offset == get_screen_offset(col, row);
    }
    else
    {
        offset = get_cursor();
    }

    if (character == '\n')
    {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    }
    else
    {
        vidmem[offset] = (unsigned char) character;
        vidmem[offset + 1] = attribute_bytes;
    }
    offset += 2;
    offset = handle_scrolling(offset);
    //offset += 2;
    set_cursor(offset);
}

int get_screen_offset(int col, int row)
{
    return ((row * MAX_COLS) + col) * 2;
}

int get_cursor()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset));
}

int handle_scrolling(int offset)
{
    if(offset >= 2 * MAX_ROWS * MAX_COLS)
    {
        unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
        int i = 0;
        for(i = 0; i < (2 * MAX_ROWS * MAX_COLS) - (2 * MAX_COLS); i++)
        {
            vidmem[i] = vidmem[i + 2 * MAX_COLS];
        }
        offset -= 2 * MAX_COLS;
    }
    return offset;
}

void print_at(char* message, int col, int row)
{
    if(col >= 0 && row >= 0)
    {
        set_cursor(get_screen_offset(col, row));
    }
    int i = 0;
    while(message[i] != 0)
    {
        print_char(message[i], col, row, GREEN_ON_BLACK);
        i++;
    }
}

void print(char* message)
{
    print_at(message, -1, -1);
}

void clear_screen()
{
    int row = 0;
    int col = 0;
    for(row = 0; row < MAX_ROWS; row++)
    {
        for(col = 0; col < MAX_COLS; col++)
        {
            print_char(' ', col, row, GREEN_ON_BLACK);
        }
    }

    set_cursor(get_screen_offset(0, 0));    
}

void test_screen()
{
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    for(int i = 0; i < MAX_COLS * MAX_ROWS * 2 + 2; i+=2)
    {
        vidmem[i] = ' ';
        vidmem[i + 1] = 0x0f;
    }

    print_char('A', 0, 0, GREEN_ON_BLACK);
    //print_char('B', 1, 0, GREEN_ON_BLACK);
    //print_char('C', 2, 0, GREEN_ON_BLACK);
    //print_char('D', 3, 0, GREEN_ON_BLACK);
    ///print_char('E', 4, 0, GREEN_ON_BLACK);
    //print_char('F', 5, 0, GREEN_ON_BLACK);
    //print_char('G', 6, 0, GREEN_ON_BLACK);
    //print_char('H', 7, 0, GREEN_ON_BLACK);
    
    /*vidmem[1] = 'A';
    vidmem[3] = 'B';
    vidmem[5] = 'C';
    vidmem[7] = 'D';
    vidmem[9] = 'E';
    vidmem[11] = 'F';
    vidmem[13] = 'G';*/
}