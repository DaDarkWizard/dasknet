#ifndef PAGING_H
#define PAGING_H

#include <efi.h>
#include <efilib.h>
#include "allocator.h"
#include "gdt.h"

#pragma pack(1)

struct mapping_table {
    uint64_t entries[512];
};

#pragma pack()

__attribute__((aligned(4096)))
struct mapping_table pml4;

/* bitflags */
#define PAGE_BIT_P_PRESENT (1<<0)
#define PAGE_BIT_RW_WRITABLE (1<<1)
#define PAGE_BIT_US_USER (1<<2)
#define PAGE_XD_NX (1<<63)

/* bit mask for page aligned 52-bit address */
#define PAGE_ADDR_MASK 0x000ffffffffff000

/* these get updated when a page is accessed/written to */
#define PAGE_BIT_A_ACCESSED (1<<5)
#define PAGE_BIT_D_DIRTY (1<<6)

void identity_map_4kb(uint64_t logical);

extern void load_pml4(struct mapping_table * pml4);

void setup_paging();


#endif