PrintString:
    push ax
    push bx

    mov ah, 0x0e
    .Loop:
    cmp [bx], byte 0x00
    je .Exit
        mov al, [bx]
        int 0x10
        inc bx
        jmp .Loop
    .Exit:

    pop bx
    pop ax
    ret