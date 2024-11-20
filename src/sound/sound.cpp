#include <cstdint>

#include "../../include/sound.hh"
#include "../../include/apu_derived.hh"
#include "../../include/ricoh_2a03.hh"

Sound::Sound() {
    //? Init member APU object
    VM_AUDIO_OUT = new APU();
}

Sound::~Sound() {
    //? Destroy member APU object
    delete VM_AUDIO_OUT;
}

void Sound::AudioOut() {
    //TODO
}

void Sound::FilterAudio(float* audio_data) {
    //TODO
    //? NES Hardware filters (post-DAC):
    //?     -1st order HP @ 90 Hz
    //?     -1st order HP @ 440 Hz
    //?     -1st order LP @ 14 kHz
}