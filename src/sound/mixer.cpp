#include <cstdint>
#include <iostream>
#include <vector>

#include "../../include/ricoh_2a03.hh"
#include "../../include/apu_derived.hh"

using namespace std;

Mixer::Mixer() {
    //? Init each synth channel
    pulse1 = new Pulse();
    pulse2 = new Pulse();
    tri = new Triangle();
    noise = new Noise();
    dmc = new DMC();

    //? Allocate + populate lookup tables
    LUT_PULSE_OUT = new float[31];
    LUT_TND_OUT = new float[203];
    for (int i = 0; i < 31; i++) {
        LUT_PULSE_OUT[i] = 95.52f / ((8128.0f / (float)i) + 100.0f);
    }
    for (int j = 0; j < 203; j++) {
        LUT_TND_OUT[j] = 163.67f / ((24329.0f / (float)j) + 100.0f);
    }
}

Mixer::~Mixer() {
    //? Destroy each synth channel
    delete pulse1;
    delete pulse2;
    delete tri;
    delete noise;
    delete dmc;

    //? Delete LUT's
    delete[] LUT_PULSE_OUT;
    delete[] LUT_TND_OUT;
}

float Mixer::MixdownNonLinear() {
    //? Update channel volumes
    GetChannelVolumes();
    //? Calculate non-linear mix for each ch. group and sum them
    pulse_out = 95.88f / ((8128.0f / (float)(pulse1_out + pulse2_out)) + 100.0f);
    tnd_out = 159.79f / 
                ((1 / 
                    (((float)tri_out / 8227.0f) +
                    ((float)noise_out / 12241.0f) +
                    ((float)dmc_out / 22638.0f))) + 100.0f);
    output_vol = pulse_out + tnd_out;
    return output_vol;
}

float Mixer::MixdownLUT() {
    //? Update channel volumes
    GetChannelVolumes();
    //? Fetch Lookup Table value for each volume group
    pulse_out = LUT_PULSE_OUT[pulse1_out + pulse2_out];
    tnd_out = LUT_TND_OUT[(3 * tri_out) +
                            (2 * noise_out) + 
                            dmc_out];
    output_vol = pulse_out + tnd_out;
    return output_vol;
}

float Mixer::MixdownLinear() {
    //? Update channel volumes
    GetChannelVolumes();
    //? Apply linear scale to each ch. group and sum them
    pulse_out = 0.00752f * (float)(pulse1_out + pulse2_out);
    tnd_out = (0.00851f * (float)tri_out) + 
                (0.00494f * (float)noise_out) + 
                (0.00335 * (float)dmc_out);
    output_vol = pulse_out + tnd_out;
    return output_vol;
}

void Mixer::GetChannelVolumes() {
    //TODO: read channel volumes from APU registers
    pulse1_out = 0;
    pulse2_out = 0;
    tri_out = 0;
    noise_out = 0;
    dmc_out = 0;
}

void Mixer::ChangeMixdownMode(MIX_APPROX_MODE mm) {
    if (mm == NONLINEAR) {
        MIX_FUNC = MixdownNonLinear;
    }
    else if (mm == LOOKUP) {
        MIX_FUNC = MixdownLUT;
    }
    else if (mm == LINEAR) {
        MIX_FUNC = MixdownLinear;
    }
    else {
        cout << "ERROR (in Mixer::ChangeMixdownMode): invalid parameter for Mix Approximation Mode - [" << mm << "]. Continuing...\n";
        return;
    }

    mix_mode = mm;
}

vector<bool> Mixer::GetChannelMuteStatuses() {
    vector<bool> mutes(5, false);
    
    mutes[0] = pulse1->IsMuted();
    mutes[1] = pulse2->IsMuted();
    mutes[2] = tri->IsMuted();
    mutes[3] = noise->IsMuted();
    mutes[4] = dmc->IsMuted();

    return mutes;
}