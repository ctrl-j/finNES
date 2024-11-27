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

using namespace std;

//? Shorthand for 8- and 16-bit unsigned integers
typedef uint8_t u8;
typedef uint16_t u16;

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

class Ricoh_2A03;

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
    u16 PC;
    u16 ADDR;
    u8 DATA;
    u8 SP;
    u8 S;
    u8 A;
    u8 X;
    u8 Y;
public:
    CPU(bool derived_caller) : Ricoh_2A03(derived_caller) {};
    ~CPU();

    void INIT();
};

class Pulse;
class Triangle;
class Noise;
class DMC;

class FrameCounter : public APU {
    public:
        //? Controlled by $4017 
        //? $4017: MI-- ---- M=Mode(0=4-step,1=5-step)
        //?                  I=IRQ inhibit flag
        
        //? Diagram for the two modes (from NESDev.org):
        //? mode 0:    mode 1:       function
        //? ---------  -----------  -----------------------------
        //?  - - - f    - - - - -    IRQ/Frame interrupt (if bit 6 is clear)
        //?  - l - l    - l - - l    Length counter and sweep
        //?  e e e e    e e e - e    Envelope and linear counter
        uint8_t frameN;


        FrameCounter();
        ~FrameCounter();

        //? Returns # corresponding to what was triggered in that frame
        //? 0= Nothing, 1= Envelope, 2= Env+Length, 3= Env+Len+IRQ
        uint8_t UpdateFrame();
        //? Returns frame # of new frame
        uint8_t AdvanceFrame();

};

class Mixer : public APU {
    public:
        Mixer();
        ~Mixer();

        enum MIX_APPROX_MODE {
            NONLINEAR,
            LOOKUP,
            LINEAR
        };
        MIX_APPROX_MODE mix_mode;
        float (Mixer::*MIX_FUNC)(); //? Function pointer for current mixdown mode

        float pulse_out, tnd_out, output_vol;
        float* LUT_TND_OUT = nullptr;
        float* LUT_PULSE_OUT = nullptr;
        vector<uint8_t> vols;


        Pulse* pulse1;
        Pulse* pulse2;
        Triangle* tri;
        Noise* noise;
        DMC* dmc;
        uint8_t pulse1_out, pulse2_out, tri_out, noise_out, dmc_out;

        //? ????????????????????????
        //? Sound "Output" Methods
        //? ????????????????????????
        //? Calculates approximate audio output level in range [0.0, 1.0]
        //? Slower but more accurate sound
        float MixdownNonLinear();
        //? Approximations of tnd_out (203 entries), pulse_out (31 entires)
        //?     (NESDev.org): "... due to the approximation of tnd_out, the numerators are 
        //?     adjusted slightly to preserve the normalized output range... The tnd_out table
        //?     is approximated (within 4%) by using a base unit close to the DMC's DAC.
        //? Faster but more rough approximation
        float MixdownLUT();
        //? Note from NESDev.org:
        //? "... results in slightly louder DMC samples, but otherwise 
        //?     fairly accurate operation since the wave channels use a small
        //?     portion of the transfer curve. The overall volume will be reduced
        //?     due to the headroom required by the DMC approximation."
        //? Fastest but roughest approximation
        float MixdownLinear();

        //? Updates digital volume level for each channel
        void GetChannelVolumes(); //TODO
        //? Returns vector of all channels' mute statuses
        //* { Pulse 1, Pulse 2, Triangle, Noise, Sampler }
        vector<bool> GetChannelMuteStatuses();

        void ChangeMixdownMode(MIX_APPROX_MODE mm);
};

class Pulse : public APU {
    //! Pulse Hardware Units:
    //!     -Volume
    //!     -Envelope
    //!     -Length Counter (5-bit)
    //!     -Sweep
    //!     -Timer (11-bit)

    public:
        bool pulse_channel;
        float duty;
        uint8_t volume; // 4-bit value
        uint8_t len_cnt;
        uint16_t period; // 11-bit value

        Pulse();
        ~Pulse();

        void SetVolume(uint8_t new_volume);
        bool IsMuted();
        void SetRawPeriod(uint16_t raw_period);
        void SetLenCntVal(uint8_t counter_val);
        void SetDuty(float duty_cycle);
        void SetSweep(uint8_t flags);
};

class Triangle : public APU {
    //! Triangle Hardware Units:
    //!     -Length Counter (5-bit)
    //!     -Linear Counter (7-bit)
    //!     -Timer (11-bit)
    public: 
        uint8_t len_cnt;
        uint16_t period; // 11-bit value

        Triangle();
        ~Triangle();

        bool IsMuted();
        void SetRawPeriod(uint16_t raw_period);
        void SetLenCntVal(uint8_t counter_val);
};

class Noise : public APU {
    //! Pulse Hardware Units:
    //!     -Volume
    //!     -Envelope
    //!     -Length Counter (5-bit)
    public:
        uint8_t volume; // 4-bit value
        uint8_t len_cnt;

        Noise();
        ~Noise();

        void SetVolume(uint8_t new_volume);
        bool IsMuted();
        void SetLenCntVal(uint8_t counter_val);
};

class DMC : public APU {
    //! From "NESDev.org" wiki:
    //!   "DPCM samples are stored as a stream of 1-bit deltas
    //!   that control the 7-bit PCM counter that the channel outputs.
    //!   A bit of 1 will increment the counter, 0 will decrement, and
    //!   it will clamp rather than overflow if the 7-bit range is exceeded."
    //! DMC Hardware Units:
    //!     -DPCM Counter (7-bit)
    //! NOTE: DPCM Samples must exist in range $C000-FFFF
    //!       at an address set in $4012 (DMC_START)
    public:
        DMC();
        ~DMC();

        bool IsMuted();
};

class APU : public Ricoh_2A03 {
public:
    APU(bool derived_caller) : Ricoh_2A03(derived_caller) {};
    ~APU();

    void INIT();

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
    Ricoh_2A03(bool derived_caller);
    ~Ricoh_2A03();

    CPU* cpu = nullptr;
    APU* apu = nullptr;

};