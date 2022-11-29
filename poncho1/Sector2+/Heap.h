#pragma once

#include "types.h"
#include "Memory.h"

struct MemorySegmentHeader
{
    uint64_t MemoryLength;
    MemorySegmentHeader* NextSegment;
    MemorySegmentHeader* PreviousSegment;
    MemorySegmentHeader* NextFreeSegment;
    MemorySegmentHeader* PreviousFreeSegment;
    bool Free;
};

struct AlignedMemorySegmentHeader {
    uint64_t MemorySegmentHeaderAddress : 63;
    bool IsAligned : 1;
};

void InitializeHeap(uint64_t heapAddress, uint64_t heapLength);
void *malloc(uint64_t size);
void free(void *memory);
void *calloc(uint64_t size);
void *realloc(void *address, uint64_t newSize);
void *aligned_alloc(uint64_t alignment, uint64_t size);