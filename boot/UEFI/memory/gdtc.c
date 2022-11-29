#include "gdt.h"

#pragma pack (1)

__attribute__((aligned(4096)))
struct gdt_table gdt_table = {
    {0, 0, 0, 0x00, 0x00, 0},  /* 0x00 null  */
    {0xffff, 0, 0, 0b10011010, 0b11001111, 0},  /* 0x08 kernel code (kernel base selector) */
    {0xffff, 0, 0, 0b10010010, 0b11001111, 0},  /* 0x10 kernel data */
    {0, 0, 0, 0x00, 0x00, 0},  /* 0x18 null (user base selector) */
    {0xffff, 0, 0, 0x92, 0xa0, 0},  /* 0x20 user data */
    {0xffff, 0, 0, 0x9a, 0xa0, 0},  /* 0x28 user code */
    {0, 0, 0, 0x92, 0xa0, 0},  /* 0x30 ovmf data */
    {0, 0, 0, 0x9a, 0xa0, 0},  /* 0x38 ovmf code */
    {0xffff, 0x0, 0x0, 0x89, 0xaf, 0},  /* 0x40 tss low */
    {0, 0, 0, 0x00, 0x00, 0},  /* 0x48 tss high */
};

#pragma pack ()

void * memzero(void * s, uint64_t n) {
    for (int i = 0; i < n; i++) ((uint8_t*)s)[i] = 0;
}

void setup_gdt() {
    memzero((void*)&tss, sizeof(tss));
    uint64_t tss_base = ((uint64_t)&tss);
    gdt_table.tss_low.base15_0 = tss_base & 0xffff;
    gdt_table.tss_low.base23_16 = (tss_base >> 16) & 0xff;
    gdt_table.tss_low.base31_24 = (tss_base >> 24) & 0xff;
    gdt_table.tss_low.limit15_0 = sizeof(tss);
    gdt_table.tss_high.limit15_0 = (tss_base >> 32) & 0xffff;
    gdt_table.tss_high.base15_0 = (tss_base >> 48) & 0xffff;

    struct table_ptr gdt_ptr = { sizeof(gdt_table)-1, (UINT64)&gdt_table };
    load_gdt(&gdt_ptr);
}