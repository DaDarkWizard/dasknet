;
; Assembly code that prints a newline to the console.
;

print_newline:
    pusha
    mov ah, 0x0e
    mov al, 10
    int 0x10
    mov al, 13
    int 0x10
    popa
    ret
