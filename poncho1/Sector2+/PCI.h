#pragma once

#include "IO.h"
#include "types.h"
#include "TextModeScreen.h"

extern uint8_t NetworkBus;
extern uint8_t NetworkSlot;

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pciConfigReadLong(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pciConfigSetWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value);
void pciConfigSetLong(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t val);
void EnumeratePCIDevices();
void DumpPCIConfig(uint8_t bus, uint8_t slot);

