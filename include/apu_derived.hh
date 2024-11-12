#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "ricoh_2a03.hh"

using namespace std;

class APU;

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

        float pulse_out, tnd_out, output_vol;
        vector<uint8_t> vols;

        Pulse* pulse1 = nullptr;
        Pulse* pulse2 = nullptr;
        Triangle* tri = nullptr;
        Noise* noise = nullptr;
        DMC* dmc = nullptr;

        float OutputCompVolumes();
        vector<uint8_t> GetChannelVolumes(); //TODO
        //? Mild lowpass filter (emulates RCA output circuitry)
        void FilterAudio(float* audio_data); //TODO
};

class Pulse : public APU {
    //! Pulse Hardware Units:
    //!     -Volume
    //!     -Envelope
    //!     -Length Counter (5-bit)
    //!     -Sweep
    //!     -Timer (11-bit)

    public:
        Pulse();
        ~Pulse();
};

class Triangle : public APU {
    //! Triangle Hardware Units:
    //!     -Length Counter (5-bit)
    //!     -Linear Counter (7-bit)
    //!     -Timer (11-bit)
    public: 
        Triangle();
        ~Triangle();
};

class Noise : public APU {
    //! Pulse Hardware Units:
    //!     -Volume
    //!     -Envelope
    //!     -Length Counter (5-bit)
    public:
        Noise();
        ~Noise();
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
};