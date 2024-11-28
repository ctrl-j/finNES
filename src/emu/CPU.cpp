#include <cstdint>

#include "../../include/cpu.hh"
#include "../../include/ricoh_2a03.hh"
#include "../../include/nes.hh"

//! ///////////////////////// !//
//!--Central Processing Unit--!//
//! ///////////////////////// !//
CPU::~CPU(){
    
}

void CPU::INIT() {
    PC = 0xFFFC;
    ADDR = 0x0;
    DATA = 0x0;
    SP = 0xFF;
    S = 32;
    A = 0;
    X = 0;
    Y = 0;
}