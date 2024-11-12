#pragma once

#include <cstdint>
#include <vector>

#include "ricoh_2a03.hh"
#include "apu_derived.hh"

using namespace std;

class Sound {
    public:
        bool APU_ACTIVE; //? 0= not active (no audio), 1= active (output audio)
        float volume_global;
        APU* VM_AUDIO_OUT;

        Sound();
        ~Sound();

        //? Reads current state of VM APU, sends output to audio device accordingly
        void AudioOut(); //TODO
        //? Mild lowpass filter (emulates RCA output circuitry)
        void FilterAudio(float* audio_data); //TODO
};