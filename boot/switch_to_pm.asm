[bits 16]
; Switch to protected mode

switch_to_pm:

    cli                         ; Make a switch of interupts to
                                ; Prevent them from running crazy


    lgdt [gdt_descriptor]       ; Load the descriptor table


    mov eax, cr0                ; Set the first bit of CR0,
    or eax, 0x1                 ; a control register, to make
    mov cr0, eax                ; the switch

    jmp CODE_SEG:init_pm        ; Make a far jump to our 32 bit
                                ; code. Also forces the CPU to flush
                                ; its cache of instructions, as that could cause
                                ; problems

[bits 32]
; Woot, 32 bit time
init_pm:

    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM

