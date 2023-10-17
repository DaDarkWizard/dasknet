#pragma once

#include "types.h"
#include "TextModeScreen.h"

struct MemoryMapEntry
{
    uint64_t BaseAddress;
    uint64_t RegionLength;
    uint32_t RegionType;
    uint32_t ExtendedAttributes;
};

void PrintMemoryMap(MemoryMapEntry *memory_map, uint16_t position);

MemoryMapEntry** GetUsableMemoryRegions();

extern uint8_t MemoryRegionCount;
extern uint8_t UsableMemoryRegionCount;