#include <efi.h>
#include <efilib.h>
#include "gop.h"
#include "tools.h"
#include "stdlib.h"
 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;


    BS = SystemTable->BootServices;
    //InitializeLib(ImageHandle, SystemTable);

    SetupTools(ImageHandle, SystemTable);
 
    /* Store the system table for future use in other functions */
    ST = SystemTable;
 
    /* Say hi */
    Status = PrintLn(L"Hello World (Dasker)"); // EFI Applications use Unicode and CRLF, a la Windows
    if (EFI_ERROR(Status))
        return Status;

    setup_GOP();
    vprintf("Finished setting up GOP. Now let's load some images.\r\n", 0xffffff);
 
    /* Now wait for a keystroke before continuing, otherwise your
       message will flash off the screen before you see it.
 
       First, we need to empty the console input buffer to flush
       out any keystrokes entered before this point */
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;
 
    /* Now wait until a key becomes available.  This is a simple
       polling implementation.  You could try and use the WaitForKey
       event instead if you like */
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;
 
    return Status;
}
