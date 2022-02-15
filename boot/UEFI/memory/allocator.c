#include "allocator.h"

static uint64_t nextFreePage = 0;
static EFI_MEMORY_DESCRIPTOR *pageAllocationDescriptor = NULL;

void setup_uefi_memory(EFI_HANDLE ImageHandle)
{
    EFI_STATUS Status;      // Variable to store the return status EFI calls.

    // Zero our descriptor.
    uefi_mmap.nbytes = 0;
    uefi_mmap.map = NULL;
    uefi_mmap.mapkey = 0;
    uefi_mmap.desc_size = 0;
    uefi_mmap.desc_version = 0;

    // Make a call to bootservices to get the map and descriptor size.
    Status = ST->BootServices->GetMemoryMap(
                            &uefi_mmap.nbytes,
                            uefi_mmap.map,
                            &uefi_mmap.mapkey,
                            &uefi_mmap.desc_size,
                            &uefi_mmap.desc_version
                            );

    ASSERT(Status == EFI_BUFFER_TOO_SMALL);

    // Allocate enough space for the map + enough space for the new descriptors from the allocations of the map
    // (one new descriptor for map + one new descriptor if a region gets split).
    uefi_mmap.map = (EFI_MEMORY_DESCRIPTOR *) AllocatePool (uefi_mmap.nbytes + 2 * uefi_mmap.desc_size);

    // Call bootservices to actually pull the map this time.
    Status = ST->BootServices->GetMemoryMap(
                            &uefi_mmap.nbytes,
                            uefi_mmap.map,
                            &uefi_mmap.mapkey,
                            &uefi_mmap.desc_size,
                            &uefi_mmap.desc_version
                            );

    if (EFI_ERROR(Status))
    {
        ST->BootServices->FreePool(uefi_mmap.map);
        uefi_mmap.map = NULL;
        ST->BootServices->Exit(ImageHandle, Status, 0, NULL);
    }

    // Calculate a "good place" to start allocating pages at.
    // This is an ok algorithm, but has *many* issues.
    // TODO: maybe setup a way to free pages.
    uint64_t bestPoolStart = 0;
    uint64_t bestNumPages = 0;

    for(int i = 0; i < uefi_mmap.nbytes; i+= uefi_mmap.desc_size)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)&uefi_mmap.map[i];
        if(desc->Type != EfiConventionalMemory) continue;
        if(desc->NumberOfPages > bestNumPages)
        {
            bestNumPages = desc->NumberOfPages;
            bestPoolStart = desc->PhysicalStart;
            pageAllocationDescriptor = desc;
        }
    }

    nextFreePage = bestPoolStart;

    Status = ST->BootServices->ExitBootServices(ImageHandle, uefi_mmap.mapkey);
    if(EFI_ERROR(Status))
    {
        ST->BootServices->FreePool(uefi_mmap.map);
        uefi_mmap.map = NULL;
        ST->BootServices->Exit(ImageHandle, Status, 0, NULL);
    }
}

void* alloc_page()
{
    if(pageAllocationDescriptor == NULL)
    {
        return NULL;
    }
    if(nextFreePage >= pageAllocationDescriptor->PhysicalStart + pageAllocationDescriptor->NumberOfPages * 4096)
    {
        return NULL;
    }
    nextFreePage += 4096;
    return (void*)(nextFreePage - 4096);
}