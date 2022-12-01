#include "MemoryMap.h"


uint8_t UsableMemoryRegionCount;

MemoryMapEntry *UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry *memory_map, uint16_t position)
{
    set_cursor_position(position);

    print_string("Memory Base: 0x"); print_string(HexToString(memory_map->BaseAddress));
    set_cursor_position(position + 80);
    print_string("Region Length: "); print_string(IntegerToString(memory_map->RegionLength));
    set_cursor_position(position + 160);
    print_string("Region Type: "); print_string(IntegerToString(memory_map->RegionType));
    set_cursor_position(position + 240);
    print_string("Memory Attributes: "); print_string(HexToString(memory_map->ExtendedAttributes));
}

bool MemoryRegionsGot = false;

MemoryMapEntry** GetUsableMemoryRegions() {
    if(MemoryRegionsGot)
    {
        return UsableMemoryRegions;
    }

    uint8_t usable_index = 0;
    for(uint8_t i = 0; i < MemoryRegionCount; i++)
    {
        MemoryMapEntry *memMap = (MemoryMapEntry*)0x5000;
        memMap += i;
        if (memMap->RegionType == 1)
        {
            UsableMemoryRegions[usable_index] = memMap;
            usable_index++;
        }
    }

    UsableMemoryRegionCount = usable_index;
    MemoryRegionsGot = true;
    return UsableMemoryRegions;
}
