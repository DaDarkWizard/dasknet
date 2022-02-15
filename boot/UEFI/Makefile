
HEADERS = lib.h efimacros.h memory/allocator.h
OBJ = hello.o efimacros.o data.o allocator.o gdtc.o gdt_asm.o paging.o syscall.o interrupts.o gdt.o
OPTIONS = -ffreestanding -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -ggdb

BOOTX64.EFI: ${OBJ}
	x86_64-w64-mingw32-gcc  -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI ${OBJ}
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT

hello.o: hello.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o hello.o hello.c

efimacros.o: efimacros.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o efimacros.o efimacros.c

data.o: data.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o data.o data.c

allocator.o: memory/allocator.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o allocator.o memory/allocator.c

gdtc.o: memory/gdtc.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o gdtc.o memory/gdtc.c

gdt_asm.o: memory/gdt_asm.c
	x86_64-w64-mingw32-gcc -masm=intel ${OPTIONS} -c -o gdt_asm.o memory/gdt_asm.c

gdt.o: memory/gdt.asm
	nasm memory/gdt.asm -f elf64 -o gdt.o

paging.o: memory/paging.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o paging.o memory/paging.c

syscall.o: interrupts/syscall.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o syscall.o interrupts/syscall.c

interrupts.o: interrupts/interrupts.c ${HEADERS}
	x86_64-w64-mingw32-gcc ${OPTIONS} -c -o interrupts.o interrupts/interrupts.c

clean:
	rm -f ${OBJ} BOOTX64.EFI fat.img