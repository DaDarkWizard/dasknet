OUTPUT_FORMAT(binary)


ENTRY(_start)

INPUT
(
    ExtendedProgram.o
    Kernel.o
    binaries.o
    TextModeScreen.o
    IO.o
    IDT.o
    Keyboard.o
    MemoryMap.o
    Heap.o
    Memory.o
    PCI.o
    Ethernet.o
)

OUTPUT
(
    Kernel.bin
)

SECTIONS
{
    . = 0x8000;

    .text : ALIGN(0x1000)
    {
        *(.text)
    }

    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _idt = .;
        . = . + 0x1000;
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
}
