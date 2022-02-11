[bits 32]
; Define screen constants
VIDEO_MEMORY equ 0xb8000
TEXT_COLOR equ 0x0a

; Print Null terminated string from EDX
print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY
print_string_pm_loop:
    mov al, [ebx]
    mov ah, TEXT_COLOR

    cmp al, 0
    je  print_string_pm_done

    mov [edx], ax
    add ebx, 1
    add edx, 2

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret
