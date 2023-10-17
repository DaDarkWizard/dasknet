
export GCC=x86_64-w64-mingw32-gcc
export CFLAGS=-ffreestanding -I/usr/include/gnu-efi-3.0.17/inc -I/usr/include/gnu-efi-3.0.17/inc/x86_64 -I/usr/include/gnu-efi-3.0.17/inc/protocol -c
export LDFLAGS=-nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main

MODULES = lib

all:
	for dir in $(MODULES); do\
		(cd $$dir; ${MAKE} all); \
	done
	${MAKE} fat.img

fat.img: BOOTX64.EFI
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT

BOOTX64.EFI: hello.o data.o gop.o tools.o
	$(GCC) $(LDFLAGS) -o BOOTX64.EFI hello.o data.o gop.o tools.o ./lib/*.o /usr/include/gnu-efi-3.0.17/x86_64/gnuefi/libgnuefi.a /usr/include/gnu-efi-3.0.17/x86_64/lib/libefi.a

hello.o: hello.c
	$(GCC) $(CFLAGS) -o hello.o hello.c

tools.o: tools.c
	$(GCC) $(CFLAGS) -o tools.o tools.c

gop.o: gop.c charset.h
	$(GCC) $(CFLAGS) -o gop.o gop.c

data.o: gnu-efi/data.c
	$(GCC) $(CFLAGS) -o data.o gnu-efi/data.c 

.PHONY:

clean:
	rm -rf **/*.o **/*.EFI *.o *.EFI

run: all
	qemu-system-x86_64 -L OVMF_dir -drive if=pflash,file=../include/OVMF.fd,format=raw,index=0,media=disk -drive file=fat.img,format=raw,index=0,media=disk
