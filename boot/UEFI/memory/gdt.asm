[bits 64]

global load_gdt
load_gdt:
    lgdt [gdt_value]
    mov ax, TSS_SEG
    ltr ax
    mov ax, DATA_SEG    ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, CODE_SEG
    push rax
    lea  rax, [abs $ + end_load_gdt]
    push rdi
    a64 retf

    
end_load_gdt:
    leaveq
    retq



gdt_value:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0


gdt_code:
; base=0x0, limit=0xfffff
    ; 1st flags: (present)1 (privilege)00 (descriptor type)1 -> 1001b
    ; type flags: (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
    ; 2nd flags: (granularity)1 (32-bit default)1 (64-big seg)0 (AVL)0 -> 1100b
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    ; Same as code segment except for the type flags:
    ; type flags: (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010b
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

;{0, 0, 0, 0x89, 0xa0, 0},  /* 0x40 tss low */
;    {0, 0, 0, 0x00, 0x00, 0},  /* 0x48 tss high */

gdt_tss_low:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10001001b
    db 10101111b
    db 0x0

gdt_tss_high:
    dw 0x0000
    dw 0x0
    db 0x0
    db 00000000b
    db 00000000b
    db 0x0

gdt_end:

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
TSS_SEG equ gdt_tss_low - gdt_start