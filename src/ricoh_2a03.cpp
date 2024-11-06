//! This file will define what is common between the APU and CPU

#include "../include/ricoh_2a03.hh"

/* //************************************
* Constructors and Destructors          *
* for Ricoh class core, APU, and CPU    *
* *///***********************************

Ricoh_2A03::Ricoh_2A03() {

}

Ricoh_2A03::~Ricoh_2A03() {
    
}

APU::APU() {

}
APU::~APU(){

}

CPU::CPU() {

}
CPU::~CPU(){
    
}

/* //************************
* Ricoh class core routines *
* *///***********************


/* //***********
* CPU routines *
* *///**********


/* //***********
* APU routines *
* *///**********

//? Internal methods
uint8_t APU::ReadReg() {

}

uint8_t APU::WriteReg() {
    
}

//? General APU functionality
void APU::CopyData_OAM_DMA() {

}

void APU::ChanEnable() {

}

//? Methods relevant to multiple channels
void APU::SQ_NOISE_SetVolume(uint8_t new_volume) {

}

void APU::SQ_TRI_SetPeriodLO(uint8_t period_low_byte) {

}

void APU::SQ_TRI_SetPeriodHI(uint8_t period_high_byte) {

}

void APU::SQ_TRI_NOISE_SetLenCntVal(uint8_t counter_val) {

}

//? Square/Pulse wave channels (2 channels)
void APU::SQ_SetDuty(uint8_t duty_cycle) {

}

void APU::SQ_SetSweep(uint8_t flags) {
    //? Sweep unit flags (two sets of flags in SQ1_SWEEP, SQ2_SWEEP)
    //? EPPP.NSSS
    //? E--- ---- Enabled flag
    //? -PPP ---- Divider period = P+1 half-frames
    //? ---- N--- Negate flag (0: add to period, sweep to low freq; 1: sub from period, sweep to high freq)
    //? ---- -SSS Shift count (# of bits) i.e. If SSS=0, behaves like E=0

    //! MUTING:
    //! Two conditions can cause muting in the sweep units
    //!     1. If the CURRENT PERIOD is less than 8, sweep unit mutes the channel
    //!     2. If the TARGET PERIOD is EVER greater than $7FF, sweep unit mutes the channel
}