#pragma once

#include <cstdint>

#include "../include/display.hh"
#include "../include/sound.hh"
#include "../include/ricoh_2a03.hh"
#include "../../include/memory.hh"
#include "../include/ppu.hh"
#include "../../include/apu_derived.hh"

//? Shorthand for 8- and 16-bit unsigned integers
typedef uint8_t u8;
typedef uint16_t u16;

class NES {
public:
    Display* _VID_OUT;
    Sound* _AUD_OUT;
    
    Memory* _MEM;
    Ricoh_2A03* _CORE;
    CPU* _CPU;
    APU* _APU;
    PPU* _PPU;

    NES();
    ~NES();

    //? Get bit value at {bit_index} of byte(s) {data}
    //? If {addr} is true and single-byte, then {data} (or {data}[0]) is used as an address for _MEM->ReadBit()
    //? If {addr} is false and single-byte, then bit is extracted from {data} (or {data[0]}) itself
    //? If multi-byte, {addr} is ignored; bit indexed from within byte array
    u8 GetBit(MDATA data, uint8_t bit_idx, bool addr);
    //? Reads from memory with requested params, after validation
    //? Caller must eventually free any multi-byte MDATA pointers
    MDATA* MemRead(uint16_t addr, uint16_t len);
};

extern NES* VM;