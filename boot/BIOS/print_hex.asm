; program for converting various datatypes to hex
; input: dx has number

print_hex_short:
    pusha
    mov bx, HEX_OUT + 6
    print_hex_short_loop:
    sub bx, 1
    cmp bx, HEX_OUT + 1
    je  print_hex_short_end
    mov ax, dx
    and ax, 0x000f
    shr dx, 4
    cmp ax, 0x0009
    jle  print_hex_short_num
    print_hex_short_char:
    add ax, 87
    jmp print_hex_short_endchar
    print_hex_short_num:
    add ax, 48
    print_hex_short_endchar:
    mov byte [bx], al
    jmp print_hex_short_loop
    print_hex_short_end:
    mov bx, HEX_OUT
    call print_string
    popa
    ret

; global variables
HEX_OUT: db '0x0000',0

