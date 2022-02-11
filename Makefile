
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)


OBJ = ${C_SOURCES:.c=.o}

all: floppy.img

floppy.img: dasknet-img
	dd if=dasknet-img of=floppy.img conv=notrunc

dasknet-img: boot/boot_sect.bin kernel.bin 
	cat $^ > dasknet-img

kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary --ignore-unresolved-symbol _GLOBAL_OFFSET_TABLE_

%.o : %.c ${HEADERS}
	gcc -m32 -ffreestanding -c $< -o $@

%.o : %.asm
	nasm $< -f elf32 -o $@

%.bin : %.asm
	nasm $< -f bin -I '../../16bit/' -o $@

clean:
	rm -fr *.bin *.dis *.o dasknet.img
	rm -fr kernel/*.o boot/*.bin drivers/*.o
