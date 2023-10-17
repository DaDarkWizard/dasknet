
PROGRAM_SPACE equ 0x8000

ReadDisk:

    mov ah, 0x02
    mov bx, PROGRAM_SPACE
    mov al, 64; read ~= 64000 bytes
    mov dl, [BOOT_DISK]
    mov ch, 0x00 ;cylinder 0
    mov dh, 0x00 ;head 0
    mov cl, 0x02 ;sector 0

    int 0x13

    jc DiskReadFailed

    ret

BOOT_DISK:
    db 0

DiskReadErrorString:
    db 'Disk Read Failed',10,13,0

DiskReadFailed:
    mov bx, DiskReadErrorString
    call PrintString
    jmp $