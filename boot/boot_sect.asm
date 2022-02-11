; Read some sectors from the boot disk using our disk_read function
[ org 0x7c00 ]
KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl    ; BIOS stores our boot drive in DL , so it ’s
                            ; best to remember this for later.

    mov bp, 0x9000          ; Here we set our stack safely out of the
    mov sp, bp              ; way , at 0x9000
    
    
    mov bx, WELCOME_TEXT
    call print_string

    call load_kernel

    call switch_to_pm       ; Call that never returns.

    jmp $

%include "boot/BIOS/print_string.asm" ; Re - use our print_string function
%include "boot/BIOS/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/printing/print_screen.asm" ; Re - use our print_string function
%include "boot/switch_to_pm.asm"

[bits 16]

; load_kernel
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret

[bits 32]

; This is where we go after entering protected mode
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    call KERNEL_OFFSET

    jmp $

; Include our new disk_load function
; Global variables
BOOT_DRIVE: db 0
WELCOME_TEXT: db 'Welcome to Dasknet!',10,'Shifting to 32 bit protected mode:',10,0
MSG_PROT_MODE: db "Successfully in 32-bit Protected Mode",0
MSG_LOAD_KERNEL: db "Loading kernel into memory.",10,0
; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55