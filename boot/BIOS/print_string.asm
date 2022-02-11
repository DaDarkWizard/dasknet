

print_string:   ; Function that prints a string. Note, address is stored in BX.
    pusha
    mov ah, 0x0e
print_string_loop:
    mov byte al, [bx]
    cmp al, 0
    je  print_string_end
    cmp al, 10
    je  print_string_newline
    int 0x10
    add bx, 1
    jmp print_string_loop
print_string_newline:
    mov al, 10
    int 0x10
    mov al, 13
    int 0x10
    add bx, 1
    jmp print_string_loop
print_string_end:
    popa
    ret
