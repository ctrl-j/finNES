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

Pulse::Pulse() {

}

Triangle::Triangle() {

}

Noise::Noise() {

}

DMC::DMC() {

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

Pulse::~Pulse() {

}

Triangle::~Triangle() {

}

Noise::~Noise() {

}

DMC::~DMC() {

}

float Mixer::MixdownNonLinear() {
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

void Pulse::SetVolume(uint8_t new_volume) {
    //? Volume is 4-bit value [0, 15]
    if (new_volume > 15) {
        cout << "WARNING (in Pulse::SetVolume): illegal value for new_volume - [" << new_volume << "]. Continuing...\n";
        return;
    }

    //? Set appropriate volume register

}

void Noise::SetVolume(uint8_t new_volume) {
    //? Volume is 4-bit value [0, 15]
    if (new_volume > 15) {
        cout << "WARNING (in Noise::SetVolume): illegal value for new_volume - [" << new_volume << "]. Continuing...\n";
        return;
    }

    //? Set appropriate volume register

}

void Pulse::SetRawPeriod(uint16_t raw_period) {
    //? Set SQ*_LO + SQ*_HI registers
    
}

void Triangle::SetRawPeriod(uint16_t raw_period) {
    //? Set TRI_LO + TRI_HI registers
}

void Pulse::SetLenCntVal(uint8_t counter_val) {
    //? Set LENGTH_CNT register
}

void Triangle::SetLenCntVal(uint8_t counter_val) {
    //? Set LENGTH_CNT register
}

void Noise::SetLenCntVal(uint8_t counter_val) {
    //? Set LENGTH_CNT register
}

void Pulse::SetDuty(float duty_cycle) {
    //? In registers $4000 (Pulse 1), $4004 (Pulse 2)
    //? %DD------ Two bit value [12.5%, 25%, 50%, 75%]
    //? Check percentage vals first
    //? Also allows for using "binary"/register-accurate vals
    if (duty_cycle == 12.5f || (int)duty_cycle == 0) {
        //TODO
    }
    else if (duty_cycle == 25.0f || (int)duty_cycle == 1) {
        //TODO
    }
    else if (duty_cycle == 25.0f || (int)duty_cycle == 2) {
        //TODO
    }
    else if (duty_cycle == 25.0f || (int)duty_cycle == 3) {
        //TODO
    }
    //? Invalid duty cycle value
    else {
        cout << "WARNING (in Pulse::SetDuty, Pulse # " << pulse_channel << "): illegal value for Duty Cycle - [" << duty_cycle << "]. Continuing...\n";
    }
}

void Pulse::SetSweep(uint8_t flags) {
    //? Sweep unit flags (two sets of flags in SQ1_SWEEP, SQ2_SWEEP)
    //? EPPP.NSSS
    //? E--- ---- Enabled flag
    //? -PPP ---- Divider period = P+1 half-frames
    //? ---- N--- Negate flag (0: add to period, sweep to low freq; 1: sub from period, sweep to high freq)
    //? ---- -SSS Shift count (# of bits) i.e. If SSS=0, behaves like E=0

    //! MUTING:
    //! Two conditions can cause muting in the sweep units
    //!     1. If the CURRENT PERIOD is less than 8, sweep unit mutes the channel
    //!     2. If the TARGET PERIOD is EVER greater than $7FF, sweep unit mutes the channel
}