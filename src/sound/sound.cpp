#include <cstdint>

#include "../../include/sound.hh"
#include "../../include/ricoh_2a03.hh"

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