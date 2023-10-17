#include "tools.h"

char __printf_buffer_[180];
unsigned short __printf_buffer_2[180];

EFI_HANDLE GlobalImageHandle;
EFI_SYSTEM_TABLE *GlobalSystemTable;

void SetupTools(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    GlobalImageHandle = ImageHandle;
    GlobalSystemTable = SystemTable;
}

EFI_STATUS PrintLn(short unsigned int* val)
{
   GlobalSystemTable->ConOut->OutputString(ST->ConOut, val);
   return GlobalSystemTable->ConOut->OutputString(ST->ConOut, L"\r\n");
}