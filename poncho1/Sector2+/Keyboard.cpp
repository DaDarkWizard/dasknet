#include "Keyboard.h"

bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint8_t LastScanCode = 0;

void StandardKeyboardHandler(uint8_t scan_code, uint8_t chr)
{
    if(chr != 0)
    {
        if(LeftShiftPressed || RightShiftPressed)
        {
            print_char(chr - 32);
        }
        else
        {
            print_char(chr);
        }
    }
    else
    {
        switch (scan_code)
        {
            case 0x8e:
                set_cursor_position(get_cursor_position() - 1);
                print_char(' ');
                set_cursor_position(get_cursor_position() - 1);
                break;
            case 0x2a:
                LeftShiftPressed = true;
                break;
            case 0xaa:
                LeftShiftPressed = false;
                break;
            case 0x36:
                RightShiftPressed = true;
                break;
            case 0xB6:
                RightShiftPressed = false;
                break;
            case 0x9C:
                print_string("\r\n");
                break;
            default:
                break;
        }
    }
}

void KeyboardHandler0xE0(uint8_t scan_code)
{
    switch (scan_code)
    {
    case 0x50:
        set_cursor_position(get_cursor_position() + VGA_WIDTH);
        break;
    case 0x48:
        set_cursor_position(get_cursor_position() - VGA_WIDTH);
        break;
    default:
        break;
    }
}

void KeyboardHandler(uint8_t scan_code, uint8_t chr)
{
    switch (LastScanCode)
    {
        case 0xE0:
            KeyboardHandler0xE0(scan_code);
            break;
        
        default:
            StandardKeyboardHandler(scan_code, chr);
            break;
    }
    LastScanCode = scan_code;
}