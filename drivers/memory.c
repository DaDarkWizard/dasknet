#include "memory.h"

unsigned long long getFreeMemorySize()
{
    static unsigned long long size = 0;
    
    // Memory already counted.
    if(size > 0)
    {
        return size;
    }


}

  EFI_STATUS                  Status;
  EFI_MEMORY_DESCRIPTOR       *EfiMemoryMap;
  UINTN                       EfiMemoryMapSize;
  UINTN                       EfiMapKey;
  UINTN                       EfiDescriptorSize;
  UINT32                      EfiDescriptorVersion;
 
  //
  // Get the EFI memory map.
  //
  EfiMemoryMapSize  = 0;
  EfiMemoryMap      = NULL;
  Status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);
 
  //
  // Use size returned for the AllocatePool.
  //
  EfiMemoryMap = (EFI_MEMORY_DESCRIPTOR *) AllocatePool (EfiMemoryMapSize + 2 * EfiDescriptorSize);
  ASSERT (EfiMemoryMap != NULL);
  Status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );
  if (EFI_ERROR (Status)) {
    FreePool (EfiMemoryMap);
  }
 
  //
  // Get descriptors
  //
  EFI_MEMORY_DESCRIPTOR       *EfiEntry = EfiMemoryMap;
  do {
    // ... do something with EfiEntry ...
    EfiEntry    = NEXT_MEMORY_DESCRIPTOR (EfiEntry, EfiDescriptorSize);
  } while((UINT8*)EfiEntry < (UINT8*)EfiMemoryMap + EfiMemoryMapSize);