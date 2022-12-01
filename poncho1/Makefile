CC = ../../gcc/bin/x86_64-elf-g++
ARGS = -Ttext 0x8000 -ffreestanding -fno-rtti -fno-exceptions -lgcc -mno-red-zone -m64

ALL: bootloader.img

bootloader.img: bootloader.bin Kernel.bin
	cp bootloader.bin bootloader.img
	cat Kernel.bin >> bootloader.img
	dd if=/dev/zero of=bootloader.img bs=1 count=1 seek=65535

bootloader.bin: Sector1/bootloader.asm
	cd Sector1 && nasm bootloader.asm -f bin -o ../bootloader.bin

ExtendedProgram.o: Sector2+/ExtendedProgram.asm Sector1/print.asm Sector2+/gdt.asm Sector2+/DetectMemory.asm Sector2+/CPUID.asm Sector2+/SimplePaging.asm Sector2+/idt.asm
	cd Sector2+ && nasm ExtendedProgram.asm -f elf64 -o ../ExtendedProgram.o

binaries.o: Sector2+/binaries.asm
	cd Sector2+ && nasm binaries.asm -f elf64 -o ../binaries.o

Kernel.o: Sector2+/Kernel.cpp
	cd Sector2+ && $(CC) $(ARGS) -c "Kernel.cpp" -o ../Kernel.o

TextModeScreen.o: Sector2+/TextModeScreen.cpp Sector2+/TextModeScreen.h
	cd Sector2+ && $(CC) $(ARGS) -c "TextModeScreen.cpp" -o ../TextModeScreen.o

IO.o: Sector2+/IO.cpp Sector2+/IO.h
	cd Sector2+ && $(CC) $(ARGS) -c "IO.cpp" -o ../IO.o

IDT.o: Sector2+/IDT.cpp Sector2+/IDT.h
	cd Sector2+ && $(CC) $(ARGS) -c "IDT.cpp" -o ../IDT.o

Keyboard.o: Sector2+/Keyboard.cpp Sector2+/Keyboard.h
	cd Sector2+ && $(CC) $(ARGS) -c "Keyboard.cpp" -o ../Keyboard.o

MemoryMap.o: Sector2+/MemoryMap.cpp Sector2+/MemoryMap.h
	cd Sector2+ && $(CC) $(ARGS) -c "MemoryMap.cpp" -o ../MemoryMap.o

Heap.o: Sector2+/Heap.cpp Sector2+/Heap.h
	cd Sector2+ && $(CC) $(ARGS) -c "Heap.cpp" -o ../Heap.o

Memory.o: Sector2+/Memory.cpp Sector2+/Memory.h
	cd Sector2+ && $(CC) $(ARGS) -c "Memory.cpp" -o ../Memory.o

PCI.o: Sector2+/PCI.cpp Sector2+/PCI.h
	cd Sector2+ && $(CC) $(ARGS) -c "PCI.cpp" -o ../PCI.o

Ethernet.o: Sector2+/Ethernet.cpp Sector2+/Ethernet.h
	cd Sector2+ && $(CC) $(ARGS) -c "Ethernet.cpp" -o ../Ethernet.o

Kernel.bin: ExtendedProgram.o Kernel.o binaries.o TextModeScreen.o IO.o IDT.o Keyboard.o MemoryMap.o Heap.o Memory.o PCI.o Ethernet.o
	../gcc/bin/x86_64-elf-ld -T"link.ld"

.PHONY: run clean
run:
	qemu-system-x86_64 -hda bootloader.img

clean:
	rm -rf *.bin *.flp *.o *.tmp *.iso *.img
