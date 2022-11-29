#include "IDT.h"

extern IDT64 _idt[256];
extern uint64_t isr1;
extern "C" void LoadIDT();

void InitializeIDT()
{
    //for(uint64_t t = 0; t < 256; t++)
   // {
        _idt[1].zero = 0;
        _idt[1].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
        _idt[1].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16);
        _idt[1].offset_high = (uint32_t)(((uint64_t)&isr1 & 0xffffffff00000000) >> 32);
        _idt[1].ist = 0;
        _idt[1].selector = 0x08;
        _idt[1].types_attr = 0x8e;
    //}

    RemapPic();

    outb(0x21, 0xfd);
    outb(0xa1, 0xff);
    LoadIDT();
}

void (*MainKeyboardHandler)(uint8_t scan_code, uint8_t character);

extern "C" void isr1_handler()
{
    uint8_t scan_code = inb(0x60);
    uint8_t chr = 0;
    if(scan_code < 0x3A)
    {
        chr = KBSet1::ScanCodeLookupTable[scan_code];
    }

    if(MainKeyboardHandler != 0)
    {
        MainKeyboardHandler(scan_code, chr);
    }
    
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}