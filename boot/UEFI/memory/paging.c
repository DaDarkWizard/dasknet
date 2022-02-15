#include "paging.h"

void identity_map_4kb(uint64_t logical) {
    /* flags: page is present, user readable and writable */
    int flags = PAGE_BIT_P_PRESENT | PAGE_BIT_RW_WRITABLE | PAGE_BIT_US_USER;

    /* extract mapping table indices from virtual address */
    int pml4_idx = (logical >> 39) & 0x1ff;
    int pdp_idx = (logical >> 30) & 0x1ff;
    int pd_idx = (logical >> 21) & 0x1ff;
    int pt_idx = (logical >> 12) & 0x1ff;
    int p_idx = logical & 0x7ff;

    /* did we define a PDPT for this PML4 index? */
    if (!(pml4.entries[pml4_idx] & PAGE_BIT_P_PRESENT)) {
        /* no, so lets allocate a new page for the PDPT */
        uint64_t pdpt_alloc = (uint64_t)alloc_page();
        /* zero it - this makes the PDPT an empty table with no PDTs present */
        memzero((void*)pdpt_alloc, 4096);
        /* now update the PML4 so it contains the new PDPT */
        pml4.entries[pml4_idx] = (pdpt_alloc & PAGE_ADDR_MASK) | flags;
        /* and make sure we can also access from our kernel
         * a bit redundant since we map the entire address space,
         * but needed when you map a smaller section */
        identity_map_4kb(pdpt_alloc);
    }

    /* get the PDPT given by the PML4 index */
    struct mapping_table * pdpt =
        (struct mapping_table*)(pml4.entries[pml4_idx] & PAGE_ADDR_MASK);
    /* and repeat the same process for the PDPT */
    if (!(pdpt->entries[pdp_idx] & PAGE_BIT_P_PRESENT)) {
        uint64_t pdt_alloc = (uint64_t)alloc_page();
        memzero((void*)pdt_alloc, 4096);
        pdpt->entries[pdp_idx] = (pdt_alloc & PAGE_ADDR_MASK) | flags;
        identity_map_4kb(pdt_alloc);
    }

    /* repeat the same process for the PDT */
    struct mapping_table * pdt =
        (struct mapping_table*)(pdpt->entries[pdp_idx] & PAGE_ADDR_MASK);
    if (!(pdt->entries[pd_idx] & PAGE_BIT_P_PRESENT)) {
        uint64_t pt_alloc = (uint64_t)alloc_page();
        memzero((void*)pt_alloc, 4096);
        pdt->entries[pd_idx] = (pt_alloc & PAGE_ADDR_MASK) | flags;
        identity_map_4kb(pt_alloc);
    }

    /* get the target PT */
    struct mapping_table * pt =
        (struct mapping_table*)(pdt->entries[pd_idx] & PAGE_ADDR_MASK);
    /* and update it such that it identity maps the given page */
    if (!(pt->entries[pt_idx] & PAGE_BIT_P_PRESENT)) {
        pt->entries[pt_idx] = (logical & PAGE_ADDR_MASK) | flags;
    }
}

void setup_paging() {
    memzero((void*)&pml4, 4096);
    for (int i = 0; i < 128*1024*1024; i+=4096) {
        identity_map_4kb(i);
    }
    load_pml4(&pml4);
}