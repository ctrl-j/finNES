//! The NES's core is the Ricoh 2A03 (RP2A03), which consists of:
//!     -CPU based on MOS 6502 (without decimal mode)
//!     -APU (audio processing unit)
//!     -22 memory-mapped registers for sound gen., joystick input, and DMA
//! Register map ($4000 - $401F)
//!     $4000-4003 --   Pulse/Square 1 Waveform
//!         4000:   SQ1_VOL     -- Duty cycle, volume
//!         4001:   SQ1_SWEEP   -- Sweep control register
//!         4002:   SQ1_LO      -- Low byte of period
//!         4003:   SQ1_HI      -- High byte of period, length counter value
//!     $4004-4007 --   Pulse/Square 2 Waveform
//!         4004:   SQ2_VOL     -- Duty cycle, volume
//!         4005:   SQ2_SWEEP   -- Sweep control register
//!         4006:   SQ2_LO      -- Low byte of period
//!         4007:   SQ2_HI      -- High byte of period, length counter value
//!     $4008-400B --   Triangle Waveform
//!         4008:   TRI_LINEAR  -- Linear counter
//!         4009:   N/A         -- Unused
//!         400A:   TRI_LO      -- Low byte of period
//!         400B:   TRI_HI      -- High byte of period, length counter value
//!     $400C-400F --   Noise Waveform
//!         400C:   NOISE_VOL   -- Volume
//!         400D:   N/A         -- Unused
//!         400E:   NOISE_LO    -- Period and waveform shape
//!         400F:   NOISE_HI    -- Length counter value
//!     $4010-4013 --   DMC (delta modulation channel) a.k.a SAMPLER
//!         4010:   DMC_FREQ    -- IRQ flag, loop flag, and frequency
//!         4011:   DMC_RAW     -- 7-bit DAC
//!         4012:   DMC_START   -- Start address = $C000 + ($40 * $XX)
//!         4013:   DMC_LEN     -- Sample length = ($10 * $XX) + 1 bytes [128 * $XX + 8 samples]
//!     $4014      --   OAMDMA  -- OAM DMA (Copy 256 bytes from $xx00-$xxFF into OAM via OAMDATA ($2004))
//!     $4015      --   SND_CHN -- Sound channels enable
//!     $4016      --   JOY1    -- Joystick strobe
//!     $4017      --   JOY2    -- Frame counter control
//!     $4018-401A --   N/A     -- APU testing functionality
//!     $401C-401F --   N/A     -- Used for unfinished IRQ timer
//! /////////////////////// !//
#pragma once

#include <cstdint>
#include <vector>
#include "apu_derived.hh"

using namespace std;

//? Defining constants for easier use of APU registers
#define SQ1_VOL (uint16_t) 0x4000
#define SQ1_SWEEP (uint16_t) 0x4001
#define SQ1_LO (uint16_t) 0x4002
#define SQ1_HI (uint16_t) 0x4003
#define SQ2_VOL (uint16_t) 0x4004
#define SQ2_SWEEP (uint16_t) 0x4005
#define SQ2_LO (uint16_t) 0x4006
#define SQ2_HI (uint16_t) 0x4007
#define TRI_LINEAR (uint16_t) 0x4008
#define TRI_LO (uint16_t) 0x400A
#define TRI_HI (uint16_t) 0x400B
#define NOISE_VOL (uint16_t) 0x400C
#define NOISE_LO (uint16_t) 0x400E
#define NOISE_HI (uint16_t) 0x400F
#define DMC_FREQ (uint16_t) 0x4010
#define DMC_RAW (uint16_t) 0x4011
#define DMC_START (uint16_t) 0x4012
#define DMC_LEN (uint16_t) 0x4013
#define OAMDMA (uint16_t) 0x4014
#define SND_CHN (uint16_t) 0x4015
#define JOY1 (uint16_t) 0x4016
#define JOY2 (uint16_t) 0x4017

class Ricoh_2A03 {
private:
    //? Register vars to hold the value after reading it from memory,
    //? so that you don't have to make your own vars for them
    uint8_t SQ1_VOL_REG;
    uint8_t SQ1_SWEEP_REG;
    uint8_t SQ1_LO_REG;
    uint8_t SQ1_HI_REG;
    uint8_t SQ2_VOL_REG;
    uint8_t SQ2_SWEEP_REG;
    uint8_t SQ2_LO_REG;
    uint8_t SQ2_HI_REG;
    uint8_t TRI_LINEAR_REG;
    uint8_t TRI_LO_REG;
    uint8_t TRI_HI_REG;
    uint8_t NOISE_VOL_REG;
    uint8_t NOISE_LO_REG;
    uint8_t NOISE_HI_REG;
    uint8_t DMC_FREQ_REG;
    uint8_t DMC_RAW_REG;
    uint8_t DMC_START_REG;
    uint8_t DMC_LEN_REG;
    uint8_t OAMDMA_REG;
    uint8_t SND_CHN_REG;
    uint8_t JOY1_REG;
    uint8_t JOY2_REG;
    
public:
    Ricoh_2A03();
    ~Ricoh_2A03();

    CPU* cpu = nullptr;
    APU* apu = nullptr;

    // Returns value at system register macro
    uint8_t ReadReg();
    // Set value at system register macro. Returns 0 if success, 1 otherwise
    uint8_t WriteReg();
};

//! NOTE ABOUT CONSTRUCTOR+DESTRUCTOR CALLING ORDER:
//! (EX) Class A (parent) --> Class B (derived 1) --> Class C (derived 2)
//! Contructors:
//!     Called from Most Based --> Most Inherited
//!     A() --> B() --> C()
//! Destructors:
//!     Called from Most Inherited --> Most Based
//!     ~C() --> ~B() --> ~A()

class CPU : public Ricoh_2A03 {
private:
    //? Internal 6502 stuff
    //? ...
public:
    CPU();
    ~CPU();
};

class APU : public Ricoh_2A03 {
public:
    APU();
    ~APU();

    Mixer* MXR;
    FrameCounter* FMCNT;

    //? Public members
    vector<float> chFreqs; // Frequency for each synth channel

    //? General APU functionality
    void CopyData_OAM_DMA(); //TODO
    void ChannelEnable(); //TODO

    //? Convert audio frequency (in Hz) to a raw period 
    //? (rounded to 11-bit whole number, 0-2047)
    uint16_t FreqToRawPeriod(float fhz);
};