#include <efi.h>
#include <efilib.h>
#include "memory/gdt.h"
#include "efimacros.h"
#include "memory/allocator.h"

#include "memory/paging.h"
#include "interrupts/interrupts.h"
#include "interrupts/syscall.h"

EFI_STATUS testNumberPrinting();
EFI_STATUS print(uint16_t* buffer);
EFI_STATUS printn(int64_t value);
EFI_STATUS printun(uint64_t value);
uint64_t pow(uint32_t base, uint16_t power);


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
 
    /* Store the system table for future use in other functions */
    ST = SystemTable;
 
    ST->ConOut->ClearScreen(ST->ConOut);

    /* Say hi */
    Status = ST->ConOut->OutputString(ST->ConOut, L"Welcome to Dasknet!\r\n");
    if (EFI_ERROR(Status))
        return Status;

    // Setup the uefi memory and exit the boot services.
    
    
    setup_uefi_memory(ImageHandle);
    setup_interrupts();
    setup_gdt();

    /*while(1);

    Status = ST->ConOut->OutputString(ST->ConOut, L"Welcome to Dasknet!\r\n");
    if (EFI_ERROR(Status))
        return Status;*/

    //while (1);

    //setup_interrupts();

    //Status = ST->ConOut->OutputString(ST->ConOut, L"GDT Setup!\r\n");
    //if (EFI_ERROR(Status))
        //return Status;

    setup_paging();

    enable_sce();

    asm("mySpot:\n"
        "hlt\n"
        "jmp mySpot");

    /* Now wait for a keystroke before continuing, otherwise your
       message will flash off the screen before you see it.
 
       First, we need to empty the console input buffer to flush
       out any keystrokes entered before this point */
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;
 
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;
 
    return Status;
}

EFI_STATUS print(uint16_t* buffer)
{
    return ST->ConOut->OutputString(ST->ConOut, buffer);
}

EFI_STATUS testNumberPrinting()
{
    EFI_STATUS Status;
    Status = printn(-1234567890);
    if(EFI_ERROR(Status))
    {
        return Status;
    }
    Status = print(L"\r\n");
    if(EFI_ERROR(Status))
    {
        return Status;
    }
    Status = printun(1234567890);
    return Status;
}


EFI_STATUS printun(uint64_t value)
{
    EFI_STATUS Status;
    uint16_t buffer[21];
    for(int i = 0; i < 20; i++)
    {
        buffer[i] = ((value / pow(10, i)) % 10) + 48;
    }
    int last = 19;
    while(buffer[last] == 48)
    {
        last--;
    }
    buffer[last + 1] = 0;
    for(int i = 0; i < last / 2; i++)
    {
        uint16_t swap = buffer[i];
        buffer[i] = buffer[last - i];
        buffer[last - i] = swap;
    }
    Status = print(buffer);
    return Status;
}

EFI_STATUS printn(int64_t value)
{
    EFI_STATUS Status;
    if(value < 0)
    {
        uint16_t buffer[2] = {45, 0};
        value += 1;
        Status = print(buffer);
        if(EFI_ERROR(Status))
        {
            return Status;
        }
        Status = printun(((uint64_t)(value * -1)) + 1);
    }
    else
    {
        Status = printun((uint64_t)value);
    }
    
    return Status;
}

uint64_t pow(uint32_t base, uint16_t power)
{
    if(power == 0)
    {
        return 1;
    }
    uint64_t output = base;
    for(int i = 1; i < power; i++)
    {
        output *= base;
    }
    return output;
}