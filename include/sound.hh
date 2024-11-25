#pragma once

#include <cstdint>
#include <vector>

#include "nes.hh"
#include "ricoh_2a03.hh"

using namespace std;

//? Shorthand for 8- and 16-bit unsigned integers
typedef uint8_t u8;
typedef uint16_t u16;

class Sound {
private:
    APU* _AUD_OUT;

public:
    u8 volume_global;
    bool APU_ACTIVE; //? 0= not active (no audio), 1= active (output audio)
    

    Sound() {};
    ~Sound() {};

    //? Reads current state of VM APU, sends output to audio device accordingly
    void AudioOut(); //TODO
    //? Mild lowpass filter (emulates RCA output circuitry)
    void FilterAudio(float* audio_data); //TODO
};