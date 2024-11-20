#pragma once

#include <cstdint>

#include "nes.hh"

class Memory {
private:
    u8* RAM;    // Array of 64KB data for the system, programs, and "peripherals"
    u8* ROM;    // Pointer at +32KB into RAM[], exists for convenience

    //? Returns pointer to requested address within internal RAM array
    u8* MemAddr(u16 address_literal);

public:
    //? Reads bytes from address "start" up to and including address "end"
    //? Returns pointer to MDATA object pointer, containing the data
    MDATA* ReadData(u16 start, u16 end);
    //? Retrieve 16-bit word from $ADDR -- $(ADDR+1)
    //u16 ReadWord(u16 addr);   // Little endian
    //? Retrieve 8-bit byte from $ADDR
    u8 ReadByte(u16 addr);
    //? Retrieve bit value from memory from the byte at the following address:
    //?     bit_idxd_addr = addr + floor(8 / bit_index)
    u8 ReadBit(u16 addr, u8 bit_index);
    

    // Attempt to write array of 16-bit data

    // Attempt to write array of 8-bit data

    // Attempt to write a 16-bit word from ADDR -- (ADDR+1)
    // Returns 0 if successful, 1 otherwise
    uint8_t WriteW();
    // Attempt to write an 8-bit byte to ADDR
    // Returns 0 if successful, 1 otherwise
    uint8_t WriteB();
    // Set bit within byte
    void SetBit();
    // Clear bit within byte
    void ClearBit();
    // Toggle bit within byte
    void FlipBit();
};

//? Used to represent single- and multi- byte reads/writes
//? Automatically handles memory and stuff
class MDATA {
public:
    bool single;
    uint8_t b; // Byte
    uint8_t* ba; // Byte array
    uint8_t ban; // Byte array length

    MDATA() : single(false), b(0), ba(NULL), ban(0) {};
    MDATA(uint8_t n) : single(true), b(n), ba(NULL), ban(0) {};
    MDATA(uint8_t* na, uint8_t n) : single(false), b(0), ba(na), ban(n) {};
    ~MDATA();

    //? Allows implicit conversion to byte/byte array
    operator uint8_t() const;
    operator uint8_t*() const;
};
