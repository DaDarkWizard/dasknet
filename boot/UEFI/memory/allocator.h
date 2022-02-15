#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <efi.h>
#include <efilib.h>
#include "../efimacros.h"

struct uefi_mmap {
    uint64_t nbytes;
    EFI_MEMORY_DESCRIPTOR* map;
    uint64_t mapkey;
    uint64_t desc_size;
    uint32_t desc_version;
} uefi_mmap;

void setup_uefi_memory(EFI_HANDLE ImageHandle);

void* alloc_page();

#endif