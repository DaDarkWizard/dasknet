#include "PCI.h"

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    outl(0xCF8, address);

    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

void pciConfigSetLong(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t val)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
    outl(0xCF8, address);
    outl(0xCFC, val);
}

uint32_t pciConfigReadLong(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    outl(0xCF8, address);

    tmp = inl(0xCFC);
    return tmp;
}

uint8_t NetworkBus = 0xFF;
uint8_t NetworkSlot = 0xFF;

void EnumeratePCIDevices()
{
    print_string("Getting PCI Devices:\n");
    for(uint8_t bus = 0; bus < 0xFF; bus++)
    {
        for(uint8_t slot = 0; slot < 0x1F; slot++)
        {
            uint16_t vendor = pciConfigReadWord(bus, slot, 0, 0);
            if(vendor != 0xFFFF)
            {
                uint16_t device = pciConfigReadWord(bus, slot, 0, 2);
                print_string("Bus: "); print_string(IntegerToString(bus));
                print_string(" Slot: "); print_string(IntegerToString(slot));
                print_string(" Vendor: 0x"); print_string(HexToString(vendor));
                print_string(" Device: 0x"); print_string(HexToString(device));
                print_string("\r\n");

                if(vendor == 0x8086 && device == 0x100E) // Intel network card.
                {
                    NetworkBus = bus;
                    NetworkSlot = slot;
                }
            }
        }
    }
}

void DumpPCIConfig(uint8_t bus, uint8_t slot)
{
    for(int i = 0; i < 16; i++)
    {
        print_string("0x"); print_string(HexToString(pciConfigReadLong(bus, slot, 0, i * 0x4)));
        print_string("\r\n");
    }
}
