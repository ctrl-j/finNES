#pragma once

class Ricoh_2A03;
#include "ricoh_2a03.hh"
#include <cstdint>
#include <vector>

//? Shorthand for 8- and 16-bit unsigned integers
typedef uint8_t u8;
typedef uint16_t u16;

class Mixer;
class FrameCounter;

class APU : virtual public Ricoh_2A03 {
public:
    APU() : Ricoh_2A03(true){};
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

class FrameCounter : virtual public APU {
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


        FrameCounter() : Ricoh_2A03(true){};
        ~FrameCounter();

        //? Returns # corresponding to what was triggered in that frame
        //? 0= Nothing, 1= Envelope, 2= Env+Length, 3= Env+Len+IRQ
        uint8_t UpdateFrame();
        //? Returns frame # of new frame
        uint8_t AdvanceFrame();

};

class Pulse;
class Triangle;
class Noise;
class DMC;

class Mixer : virtual public APU {
    public:
        Mixer() : Ricoh_2A03(true){};
        void INIT();
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

class Pulse : virtual public APU {
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

        Pulse() : Ricoh_2A03(true){};
        ~Pulse();

        void SetVolume(uint8_t new_volume);
        bool IsMuted();
        void SetRawPeriod(uint16_t raw_period);
        void SetLenCntVal(uint8_t counter_val);
        void SetDuty(float duty_cycle);
        void SetSweep(uint8_t flags);
};

class Triangle : virtual public APU {
    //! Triangle Hardware Units:
    //!     -Length Counter (5-bit)
    //!     -Linear Counter (7-bit)
    //!     -Timer (11-bit)
    public: 
        uint8_t len_cnt;
        uint16_t period; // 11-bit value

        Triangle() : Ricoh_2A03(true){};
        ~Triangle();

        bool IsMuted();
        void SetRawPeriod(uint16_t raw_period);
        void SetLenCntVal(uint8_t counter_val);
};

class Noise : virtual public APU {
    //! Pulse Hardware Units:
    //!     -Volume
    //!     -Envelope
    //!     -Length Counter (5-bit)
    public:
        uint8_t volume; // 4-bit value
        uint8_t len_cnt;

        Noise() : Ricoh_2A03(true){};
        ~Noise();

        bool IsMuted();
        void SetVolume(uint8_t new_volume);
        void SetLenCntVal(uint8_t counter_val);
};

class DMC : virtual public APU {
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
        DMC() : Ricoh_2A03(true){};
        ~DMC();

        bool IsMuted();
};
