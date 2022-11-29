#include "Heap.h"


MemorySegmentHeader* FirstFreeMemorySegment;

void InitializeHeap(uint64_t heapAddress, uint64_t heapLength)
{
    FirstFreeMemorySegment = (MemorySegmentHeader*)heapAddress;
    FirstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);

    FirstFreeMemorySegment->NextSegment = 0;
    FirstFreeMemorySegment->PreviousSegment = 0;
    FirstFreeMemorySegment->NextFreeSegment = 0;
    FirstFreeMemorySegment->PreviousFreeSegment = 0;
    FirstFreeMemorySegment->Free = true;
}

void *malloc(uint64_t size)
{
    uint64_t remainder = size % 8;
    size -= remainder;
    if(remainder != 0)
    {
        size += 8;
    }

    MemorySegmentHeader *currentMemorySegment = FirstFreeMemorySegment;

    while(true)
    {
        if(currentMemorySegment->MemoryLength >= size)
        {
            if(currentMemorySegment->MemoryLength > size + sizeof(MemorySegmentHeader))
            {
                MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint64_t)currentMemorySegment + sizeof(MemorySegmentHeader) + size);

                newSegmentHeader->Free = true;
                newSegmentHeader->MemoryLength = ((uint64_t)currentMemorySegment->MemoryLength) - (sizeof(MemorySegmentHeader) + size);
                newSegmentHeader->NextFreeSegment = currentMemorySegment->NextFreeSegment;
                newSegmentHeader->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
                newSegmentHeader->NextSegment = currentMemorySegment->NextSegment;
                newSegmentHeader->PreviousSegment = currentMemorySegment;

                currentMemorySegment->NextFreeSegment = newSegmentHeader;
                currentMemorySegment->NextSegment = newSegmentHeader;
                currentMemorySegment->MemoryLength = size;
            }

            if(currentMemorySegment == FirstFreeMemorySegment)
            {
                FirstFreeMemorySegment = currentMemorySegment->NextFreeSegment;
            }
            currentMemorySegment->Free = false;
            currentMemorySegment->MemoryLength = size;
            if(currentMemorySegment->PreviousFreeSegment != 0) currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
            if(currentMemorySegment->NextFreeSegment != 0) currentMemorySegment->NextFreeSegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
            
            MemorySegmentHeader *tmpMemorySegment;
            tmpMemorySegment = currentMemorySegment->NextSegment;
            while(tmpMemorySegment != 0 && !tmpMemorySegment->Free)
            {
                tmpMemorySegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
                tmpMemorySegment = tmpMemorySegment->NextSegment;
            }
            if(tmpMemorySegment != 0)
            {
                tmpMemorySegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
            }

            tmpMemorySegment = currentMemorySegment->PreviousSegment;
            while(tmpMemorySegment != 0 && !tmpMemorySegment->Free)
            {
                tmpMemorySegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
                tmpMemorySegment = tmpMemorySegment->PreviousSegment;
            }
            if(tmpMemorySegment != 0)
            {
                tmpMemorySegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
            }


            return currentMemorySegment + 1;
        }
        if(currentMemorySegment->NextFreeSegment == 0)
        {
            return 0; // No memory remaining.
        }
        currentMemorySegment = currentMemorySegment->NextFreeSegment;
    }
    return 0; // We should never get here.
}

void CombineFreeSegments(MemorySegmentHeader *a, MemorySegmentHeader *b)
{
    if(a == 0 || b == 0) return;

    if(a < b) {
        a->MemoryLength += b->MemoryLength + sizeof(MemorySegmentHeader);
        a->NextFreeSegment = b->NextFreeSegment;
        a->NextSegment = b->NextSegment;
        b->NextSegment->PreviousSegment = a;
        MemorySegmentHeader *tmpMemorySegment = b->NextSegment;
        while(tmpMemorySegment != 0 && !tmpMemorySegment->Free)
        {
            tmpMemorySegment->PreviousFreeSegment = a;
            tmpMemorySegment = tmpMemorySegment->NextSegment;
        }
        if(tmpMemorySegment != 0)
        {
            tmpMemorySegment->PreviousFreeSegment = a;
        }
    }
    else
    {
        CombineFreeSegments(b, a);
    }
}

void free(void *address)
{
    MemorySegmentHeader *currentMemorySegment;
    
    AlignedMemorySegmentHeader *AMSH = (AlignedMemorySegmentHeader*)address - 1;
    if(AMSH->IsAligned) {
        currentMemorySegment = (MemorySegmentHeader*)(uint64_t)AMSH->MemorySegmentHeaderAddress;
    }
    else
    {    
        currentMemorySegment = ((MemorySegmentHeader*)address) - 1;
    }

    currentMemorySegment->Free = true;
    
    if(currentMemorySegment < FirstFreeMemorySegment)
    {
        FirstFreeMemorySegment = currentMemorySegment;
    }
    
    MemorySegmentHeader *tmpMemorySegment = currentMemorySegment->PreviousSegment;
    while(tmpMemorySegment != 0 && !tmpMemorySegment->Free)
    {
        tmpMemorySegment->NextFreeSegment = currentMemorySegment;
        tmpMemorySegment = tmpMemorySegment->PreviousSegment;
    }
    if(tmpMemorySegment != 0)
    {
        tmpMemorySegment->NextFreeSegment = currentMemorySegment;
    }

    tmpMemorySegment = currentMemorySegment->NextSegment;
    while(tmpMemorySegment != 0 && !tmpMemorySegment->Free)
    {
        tmpMemorySegment->PreviousFreeSegment = currentMemorySegment;
        tmpMemorySegment = tmpMemorySegment->NextSegment;
    }
    if(tmpMemorySegment != 0)
    {
        tmpMemorySegment->PreviousFreeSegment = currentMemorySegment;
    }

    while(currentMemorySegment->NextSegment != 0 && currentMemorySegment->NextSegment->Free)
    {
        CombineFreeSegments(currentMemorySegment, currentMemorySegment->NextSegment);
    }

    while(currentMemorySegment->PreviousSegment != 0 && currentMemorySegment->PreviousSegment->Free)
    {
        tmpMemorySegment = currentMemorySegment->PreviousSegment;
        CombineFreeSegments(currentMemorySegment->PreviousSegment, currentMemorySegment);
        currentMemorySegment = tmpMemorySegment;
    }
}

void *calloc(uint64_t size)
{
    void *mallocVal = malloc(size);
    memset(mallocVal, 0, size);
    return mallocVal;
}

void *calloc(uint64_t num, uint64_t size)
{
    return calloc(num * size);
}

void *realloc(void *address, uint64_t newSize)
{
    MemorySegmentHeader *oldSegmentHeader;
    
    AlignedMemorySegmentHeader *AMSH = (AlignedMemorySegmentHeader*)address - 1;
    if(AMSH->IsAligned) {
        oldSegmentHeader = (MemorySegmentHeader*)(uint64_t)AMSH->MemorySegmentHeaderAddress;
    }
    else
    {    
        oldSegmentHeader = ((MemorySegmentHeader*)address) - 1;
    }
    uint64_t smallerSize = newSize;
    if(oldSegmentHeader->MemoryLength < newSize) smallerSize = oldSegmentHeader->MemoryLength;
    void* newMem = malloc(newSize);
    memcpy(newMem, address, smallerSize);
    free(address);
    return newMem;
}

void *aligned_alloc(uint64_t alignment, uint64_t size)
{
    uint64_t alignmentRemainder = alignment % 8;
    alignment -= alignmentRemainder;
    if(alignmentRemainder != 0) alignment += 8;

    uint64_t sizeRemainder = size % 8;
    size -= sizeRemainder;
    if(sizeRemainder != 0) size += 8;

    uint64_t fullsize = size + alignment;

    void *mallocVal = malloc(fullsize);
    uint64_t address = (uint64_t)mallocVal;

    uint64_t remainder = address % alignment;
    address -= remainder;
    if(remainder != 0)
    {
        address += alignment;

        AlignedMemorySegmentHeader *AMSH = (AlignedMemorySegmentHeader*)address -1;
        AMSH->IsAligned = true;
        AMSH->MemorySegmentHeaderAddress = (uint64_t)mallocVal - sizeof(MemorySegmentHeader);
    }

    return (void*)address;
}