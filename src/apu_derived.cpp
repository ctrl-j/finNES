#include <cstdint>
#include <iostream>
#include <vector>

#include "../include/ricoh_2a03.hh"

Mixer::Mixer() {
    //? Init each synth channel
    pulse1 = new Pulse();
    pulse2 = new Pulse();
    tri = new Triangle();
    noise = new Noise();
    dmc = new DMC();
}

Pulse::Pulse() {

}

Triangle::Triangle() {

}

Noise::Noise() {

}

DMC::DMC() {

}

Mixer::~Mixer() {

}

Pulse::~Pulse() {

}

Triangle::~Triangle() {

}

Noise::~Noise() {

}

DMC::~DMC() {

}

float Mixer::OutputCompVolumes() {
    //? Get channel volumes, sum into two groups
    vols = GetChannelVolumes();
    uint8_t pulse1_out = vols[0];
    uint8_t pulse2_out = vols[1];
    uint8_t tri_out = vols[2];
    uint8_t noise_out = vols[3];
    uint8_t dmc_out = vols[4];
    
    //? Non-linear scaling to offset DAC differences
    pulse_out = 95.88f / ((8128.0f / (pulse1_out + pulse2_out)) + 100.0f);
    tnd_out = 159.79f / ((1 / ((tri_out / 8227.0f) + (noise_out / 12241.0f) + (dmc_out / 22638.0f))) + 100.0f);
    output_vol = pulse_out + tnd_out;
    
    return output_vol;
}

vector<uint8_t> Mixer::GetChannelVolumes() {
    //TODO
}

void Mixer::FilterAudio(float* audio_data) {
    //TODO
}