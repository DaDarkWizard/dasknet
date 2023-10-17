#ifndef TOOLS_H
#define TOOLS_H

#include <efi.h>
#include <efilib.h>
#include "lib/stdio.h"

extern char __printf_buffer_[180];
extern unsigned short __printf_buffer_2[180];

extern EFI_HANDLE GlobalImageHandle;
extern EFI_SYSTEM_TABLE *GlobalSystemTable;

void SetupTools(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS PrintLn(short unsigned int* val);

#define Printf(format, ...) \
    sprintf(__printf_buffer_, format, __VA_ARGS__);\
    for(int __printfx__ = 0; __printfx__ < 180; __printfx__++)\
    {\
        __printf_buffer_2[__printfx__] = __printf_buffer_[__printfx__];\
    }\
    PrintLn(__printf_buffer_2);

#endif