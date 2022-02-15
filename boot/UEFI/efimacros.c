#include <efi.h>
#include <efilib.h>

void* AllocatePool(UINTN size)
{
    void** pool;
    EFI_STATUS Status;
    Status = ST->BootServices->AllocatePool(EfiRuntimeServicesData, size, pool);
    if(EFI_ERROR(Status))
    {
        return NULL;
    }
    return *pool;
}