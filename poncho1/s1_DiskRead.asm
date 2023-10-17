
PROGRAM_SPACE equ 0x8000

ReadDisk:

    mov ah, 0x02    ; We're reading from the disk
    mov bx, PROGRAM_SPACE ; We're reading it to the program space address.
    mov al, 64; read ~= 64000 bytes
    mov dl, [BOOT_DISK] ; We're reading from boot disk
    mov ch, 0x00 ;cylinder 0 ; Choose cylinder (hda specific)
    mov dh, 0x00 ;head 0 ; Choose head (hda specific)
    mov cl, 0x02 ;sector 2 ; Start in the second sector (the first is already loaded, obviously)

    int 0x13 ; Do the read

    jc DiskReadFailed ; Jump to disk read failed if a failed condition is detected.

    ret

BOOT_DISK:
    db 0

DiskReadErrorString:
    db 'Disk Read Failed',10,13,0

DiskReadFailed:
    mov bx, DiskReadErrorString
    call PrintString
    jmp $
