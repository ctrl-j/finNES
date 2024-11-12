#include <cstdint>

#include "../include/sound.hh"
#include "../include/apu_derived.hh"
#include "../include/ricoh_2a03.hh"

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
}