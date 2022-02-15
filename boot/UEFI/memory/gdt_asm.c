#ifndef GDT_ASM_H
#define GDT_ASM_H

/*
asm(".global load_gdt\n"
"load_gdt:\n"
    "mov ax, ax\n"
    "lgdt [rcx]\n"//      ; load GDT, rdi (1st argument) contains the gdt_ptr\n"
    "mov ax, 0x40\n"//    ; TSS segment is 0x40\n"
    "ltr ax\n"//          ; load TSS\n"
    "mov ax, 0x10\n"//    ; kernel data segment is 0x10\n"
    "mov ds, ax\n"//      ; load kernel data segment in data segment registers\n"
    "mov es, ax\n"
    "mov fs, ax\n"
    "mov gs, ax\n"
    "mov ss, ax\n"
    "popq rdi\n"//        ; pop the return address\n"
    "mov rax, 0x08\n"//   ; kernel code segment is 0x08\n"
    "pushq rax\n"//       ; push the kernel code segment\n"
    "pushq rdi\n"//       ; push the return address again\n"
    //"stop:\n"
    //"hlt\n"
    //"jmp stop\n"
    "lretq\n"//           ; do a far return, like a normal return but\n"
                    //"; pop an extra argument of the stack\n"
                   ); //"; and load it into CS");*/

asm(".global load_pml4\n"
"load_pml4:\n"
    "mov rax, 0x000ffffffffff000\n"
    "and rdi, rax\n"
    "mov cr3, rdi\n"
    "ret");


asm(".global enable_sce\n"
"enable_sce:\n"
    "mov rcx, 0xc0000080\n"// ; EFER MSR
    "rdmsr\n"//               ; read current EFER
    "or eax, 1\n"//           ; enable SCE bit
    "wrmsr\n"//               ; write back new EFER
    "mov rcx, 0xc0000081\n"// ; STAR MSR
    "rdmsr\n"//               ; read current STAR
    "mov edx, 0x00180008\n"// ; load up GDT segment bases 0x0 (kernel) and 0x18 (user)
    "wrmsr\n"//               ; write back new STAR
    "ret"); //                 ; return back to C
#endif