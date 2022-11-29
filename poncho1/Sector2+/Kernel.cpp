
#include "types.h"
#include "IO.h"
#include "TextModeScreen.h"
#include "IDT.h"
#include "Keyboard.h"
#include "MemoryMap.h"
#include "TextModeColorCodes.h"
#include "Heap.h"

extern const char Test[];

extern "C" void _start() {
    clear_screen(BACKGROUND_BLACK | FOREGROUND_WHITE);
    set_cursor(0,0);
    InitializeIDT();
    MainKeyboardHandler = KeyboardHandler;

    MemoryMapEntry **usable_maps = GetUsableMemoryRegions();

    for(uint8_t i = 0; i < UsableMemoryRegionCount; i++)
    {
        MemoryMapEntry* memMap = usable_maps[i];
        PrintMemoryMap(memMap, get_cursor_position());
        print_string("\r\n\n");
    }

    InitializeHeap(0x100000, 0x100000);

    uint64_t *TestAddress = (uint64_t*)aligned_alloc(0x4000, 0x08);
    print_string(HexToString((uint64_t)TestAddress));
    print_string("\n");
    //free(TestAddress);

    uint64_t *TestAddress2 = (uint64_t*)malloc(0x4000);
    print_string(HexToString((uint64_t)TestAddress2));
    print_string("\n");
    free(TestAddress);
    
    return;
}

