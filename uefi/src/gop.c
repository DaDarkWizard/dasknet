#include "gop.h"
#include "tools.h"
#include "lib/stdio.h"
#include <efi.h>
#include <efilib.h>
#include <efibind.h>
#include "charset.h"

unsigned int* frameBufferStart;
UINTN frameBufferLength;
UINT32 pixelsPerScanLine;
UINT32 horizontalResolution;
UINT32 verticalResolution;

void setup_GOP()
{
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    
    int status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(status))
        PrintLn(L"Unable to locate GOP");
    else PrintLn(L"Located GOP");


    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes, nativeMode;

    status = gop->QueryMode(gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // this is needed to get the current video mode
    if (status == EFI_NOT_STARTED)
        status = gop->SetMode(gop, 0);
    if(EFI_ERROR(status)) {
        PrintLn(L"Unable to get native mode");
    } else {
        PrintLn(L"Found native mode");
        nativeMode = gop->Mode->Mode;
        numModes = gop->Mode->MaxMode;
    }
        char buf[160];
        unsigned short sbuf[160];
        /*sprintf(buf, "Test %d %s", 1, "test test");
        for(int j = 0; j < 30; j++)
        {
            sbuf[j] = buf[j];
        }
        //sbuf[0] = '0';
        //sbuf[1] = NULL;
        PrintLn(sbuf);
        PrintLn(L"TEST");*/
    
    int mode = 0;

    for (int i = 0; i < numModes; i++) {
        status = gop->QueryMode(gop, i, &SizeOfInfo, &info);
        
        if(info->HorizontalResolution == 1920 && info->VerticalResolution == 1080)
        {
            mode = i;
        }
    
        sprintf(buf, "mode %d width %d height %d format %d%s",
            i,
            info->HorizontalResolution,
            info->VerticalResolution,
            info->PixelFormat,
            i == nativeMode ? "(current)" : ""
        );
        for(int j = 0; j < 160; j++)
        {
            sbuf[j] = buf[j];
        }
        PrintLn(sbuf);
        //PrintLn(L"TEST");
    }

    status = gop->SetMode(gop, mode);
    if(EFI_ERROR(status)) {
        Printf("Unable to set mode %d\r\n", mode);
    } else {
        // get framebuffer
        Printf("Framebuffer address %d size %d, width %d height %d pixelsperline %d",
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferSize,
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution,
        gop->Mode->Info->PixelsPerScanLine
        );
    }
    
    frameBufferStart = (unsigned int*)(gop->Mode->FrameBufferBase);
    frameBufferLength = gop->Mode->FrameBufferSize;
    horizontalResolution = gop->Mode->Info->HorizontalResolution;
    verticalResolution = gop->Mode->Info->VerticalResolution;
    pixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

    clearscreen(0x000000ff);
    vprintf("!\"#$%&'()*+,-./0123456789:;<=>?@AB", 0x00ffffff, frameBufferStart);
}

void vprintf(const unsigned char* str, unsigned int color, unsigned int *start)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        vprintchar(str[i], color, start + i * 14);
    }
}

struct d_character {
    unsigned char value[8];
};

void vprintchar(unsigned char character, unsigned int color, unsigned int *start)
{
    unsigned short *a = ASCII_CHARSET[character];

    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 14; j++)
        {
            if((0x1 << (13 - j)) & a[i])
            {
                start[j] = color;
            }
        }
        start += pixelsPerScanLine;
    }
}

void clearscreen(unsigned int color)
{
    unsigned int *i;
    for(i = frameBufferStart; i < frameBufferStart + (unsigned int)frameBufferLength/4 ; i+=1)
    {
        *i = color;
    }
}