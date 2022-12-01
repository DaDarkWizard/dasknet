
#include "types.h"
#include "IO.h"
#include "TextModeScreen.h"
#include "IDT.h"
#include "Keyboard.h"
#include "MemoryMap.h"
#include "TextModeColorCodes.h"
#include "Heap.h"
#include "PCI.h"
#include "Ethernet.h"

extern const char Test[];

extern "C" void _start() {
    clear_screen(BACKGROUND_BLACK | FOREGROUND_WHITE);
    set_cursor(0,0);
    InitializeIDT();
    MainKeyboardHandler = KeyboardHandler;

    if(!SetupHeap())
    {
        print_string("Unable to setup heap.\r\n");
        return;
    }

    EnumeratePCIDevices();
    
    SetupEthernet();

    return;
}

