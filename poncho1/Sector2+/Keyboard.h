#pragma once

#include "types.h"
#include "TextModeScreen.h"

void KeyboardHandler(uint8_t scan_code, uint8_t chr);

extern bool LeftShiftPressed;
extern bool RightShiftPressed;
extern uint8_t LastScanCode;