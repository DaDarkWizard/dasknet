#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Attribute byte for white on black.
#define WHITE_ON_BLACK 0x0f
#define GREEN_ON_BLACK 0x0a

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void clear_screen();
void print(char* message);
void print_at(char* message, int col, int row);
void test_screen();

#endif