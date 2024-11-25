#include <cstdint>
#include <iostream>

#include "../../include/memory.hh"

u8* Memory::MemAddr(u16 address_literal) {
    return &(RAM[address_literal]);
}

u8 Memory::ReadByte(u16 addr) {
    return RAM[addr];
}

MDATA* Memory::ReadData(u16 start, u16 end) {
    if (end <= start) {
        return NULL;
    }

    //? Create MDATA object to store data+size
    MDATA* read_data = new MDATA(MemAddr(start), end - start);
    return read_data;
}

u8 Memory::ReadBit(u16 addr, u8 bit_index) {
    //? Invalid word index OR end of address space
    if (bit_index > 15 || addr == 0xFFFF) {
        return MDATA();
    }

    MDATA read_byte(0); 
    if (bit_index < 8) {
        read_byte.b = ReadByte(addr);
    }
    //? Allows bit-indexing of multi-byte data (little endian)
    else if (bit_index >= 8) {
        u8 byte_offset = bit_index / 8;
        //? Multi-byte bit index goes out of bounds; get last byte
        if (UINT16_MAX - byte_offset < addr) {
            read_byte.b = ReadByte(0xFFFF);
        }
        else {
            read_byte.b = ReadByte(addr + byte_offset);
        }
    }

    return (u8)(read_byte & (1 << (bit_index % 8)));
}