#pragma once

class Ricoh_2A03;
#include "ricoh_2a03.hh"
#include <cstdint>
#include <vector>

//? Shorthand for 8- and 16-bit unsigned integers
typedef uint8_t u8;
typedef uint16_t u16;

class CPU : virtual public Ricoh_2A03 {
private:
    //? Internal 6502 stuff
    //? ...
    u16 PC;
    u16 ADDR;
    u8 DATA;
    u8 SP;
    u8 S;
    u8 A;
    u8 X;
    u8 Y;
public:
    CPU() : Ricoh_2A03(true) {};
    ~CPU();

    void INIT();
};