CC = ../gcc/bin/x86_64-elf-g++
ARGS = -Ttext 0x8000 -ffreestanding -fno-rtti -fno-exceptions -lgcc -mno-red-zone -m64

ALL: bootloader.img

bootloader.img: bootloader.bin Kernel.bin
	cp bootloader.bin bootloader.img
	cat Kernel.bin >> bootloader.img
	dd if=/dev/zero of=bootloader.img bs=1 count=1 seek=65535

bootloader.bin: s1_bootloader.asm
	nasm s1_bootloader.asm -f bin -o bootloader.bin

ExtendedProgram.o: ExtendedProgram.asm s1_print.asm gdt.asm DetectMemory.asm CPUID.asm SimplePaging.asm idt.asm
	nasm ExtendedProgram.asm -f elf64 -o ExtendedProgram.o

binaries.o: binaries.asm
	nasm binaries.asm -f elf64 -o binaries.o

Kernel.o: Kernel.cpp
	$(CC) $(ARGS) -c "Kernel.cpp" -o Kernel.o

TextModeScreen.o: TextModeScreen.cpp TextModeScreen.h
	$(CC) $(ARGS) -c "TextModeScreen.cpp" -o TextModeScreen.o

IO.o: IO.cpp IO.h
	$(CC) $(ARGS) -c "IO.cpp" -o IO.o

IDT.o: IDT.cpp IDT.h
	$(CC) $(ARGS) -c "IDT.cpp" -o IDT.o

Keyboard.o: Keyboard.cpp Keyboard.h
	$(CC) $(ARGS) -c "Keyboard.cpp" -o Keyboard.o

MemoryMap.o: MemoryMap.cpp MemoryMap.h
	$(CC) $(ARGS) -c "MemoryMap.cpp" -o MemoryMap.o

Heap.o: Heap.cpp Heap.h
	$(CC) $(ARGS) -c "Heap.cpp" -o Heap.o

Memory.o: Memory.cpp Memory.h
	$(CC) $(ARGS) -c "Memory.cpp" -o Memory.o

PCI.o: PCI.cpp PCI.h
	$(CC) $(ARGS) -c "PCI.cpp" -o PCI.o

Ethernet.o: Ethernet.cpp Ethernet.h
	$(CC) $(ARGS) -c "Ethernet.cpp" -o Ethernet.o

Kernel.bin: ExtendedProgram.o Kernel.o binaries.o TextModeScreen.o IO.o IDT.o Keyboard.o MemoryMap.o Heap.o Memory.o PCI.o Ethernet.o
	../gcc/bin/x86_64-elf-ld -T"link.ld"

.PHONY: run clean
run:
	qemu-system-x86_64 -hda bootloader.img

clean:
	rm -rf *.bin *.flp *.o *.tmp *.iso *.img
