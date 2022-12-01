[org 0x7c00]

mov [BOOT_DISK], dl

mov bp, 0x7c00 ; Setup the stack
mov sp, bp

call ReadDisk ; Load the rest of the OS from the disk

jmp PROGRAM_SPACE ; Jump to the main part of the bootloader

%include "print.asm"
%include "DiskRead.asm"

times 510-($ - $$) db 0

dw 0xaa55
