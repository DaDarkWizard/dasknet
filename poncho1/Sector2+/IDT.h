#pragma once

#include "types.h"
#include "IO.h"
#include "TextModeScreen.h"
#include "KeyboardScanCodeSet1.h"

struct IDT64
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};

void InitializeIDT();

extern void (*MainKeyboardHandler)(uint8_t scan_code, uint8_t character);

extern "C" void isr1_handler();