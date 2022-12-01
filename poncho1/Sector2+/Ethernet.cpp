#include "Ethernet.h"

uint64_t enetaddress;
uint64_t ioaddress;

void enable_eeprom()
{
    uint32_t config = pciConfigReadLong(NetworkBus, NetworkSlot, 0, 0x04);
    config = config | 0x7;
    pciConfigSetLong(NetworkBus, NetworkSlot, 0, 0x04, config);

    // Reset the card.
    outl(ioaddress, 0x0);
    outl(ioaddress + 0x4, 0x4000000);

    // Enable eeprom
    outl(ioaddress, 0x00010);
    outl(ioaddress + 0x04, 0b00000111);
}

void lock_eeprom()
{
    outl(ioaddress, 0x00010);
    uint32_t val = inl(ioaddress + 0x04);
    outl(ioaddress + 0x04, val | 0x20);

    while(true)
    {
        print_string("\rValue: "); print_string(HexToString(val));

        val = inl(ioaddress + 0x04);
        if(val & 0x40) break;
    }

    print_string("\r\nEEPROM LOCKED");
}

void SetupEthernet()
{
    // 82540 Ethernet
    print_string("Setting up Ethernet\n");

    if(NetworkBus == 0xFF && NetworkSlot == 0xFF)
    {
        print_string("No ethernet found\n");
        return;
    }

    uint64_t address = pciConfigReadLong(NetworkBus, NetworkSlot, 0, 0x10);
    uint64_t ioaddress = 0;
    if(address & 0x0000000000000003 == (uint64_t)0x3)
    {
        print_string("64 bit card.\n");
    }
    else
    {
        print_string("32 bit card.\n");
        enetaddress = address & 0xFFFFFFF0;
    }


    clear_screen(0x0F);


    for(int i = 0; i < 6; i++)
    {
        ioaddress = pciConfigReadLong(NetworkBus, NetworkSlot, 0, 0x10 + i * 0x04);
        if(ioaddress & 0x1)
        {
            ioaddress = ioaddress & 0xFFFFFFFC;
            break;
        }
    }

    //DumpPCIConfig(NetworkBus, NetworkSlot);

    print_string("Address: "); print_string(HexToString(address ));
    print_string("\r\nENetAddress: "); print_string(HexToString(enetaddress));
    print_string("\r\nIOAddress: "); print_string(HexToString(ioaddress));
    print_string("\r\n");

    enable_eeprom();
    lock_eeprom();

    //*((uint32_t*)address) = *((uint32_t*)address) | 0x4000000;

    //outl(address, 0);
    //outl(address + 0x04, 0x4000000);

    //print_string("\nDevice: "); print_string(HexToString(*((uint16_t*)address + 1)));

    //pciConfigSetWord(NetworkBus, NetworkSlot, 0, 3, command);
    //command = pciConfigReadWord(NetworkBus, NetworkSlot, 0, 3);
    //print_string("\r\nBus after: 0x"); print_string(HexToString(command));
}
