#include <cstdint>
#include <iostream>

#include "../include/ricoh_2a03.hh"

using namespace std;

//! /////////////////////// !//
//!--Audio Processing Unit--!//
//! /////////////////////// !//
//? Internal methods
APU::APU() {
    //? Init members
    chFreqs = vector<float>(5, 0.0f); // Init each channel to 0 Hz

    //? Set APU to known initial state:
    //?     -Disable sweep, envelope, length

}

uint8_t APU::ReadReg() {

}

uint8_t APU::WriteReg() {
    
}

bool APU::IsChannelMuted(uint8_t channel_num) {
    //? General mute conditions:
    //?     -EITHER pulse channel has vol=0 then MUTE them
    //?     -Tri, Noise, or DCM has vol=0 then MUTE them
    //?     -Length counter == 0

    //? Pulse/Square 1 and 2
    if (channel_num < 2) {
        //? FREQUENCY; 
        //? SWEEP UNIT; current period is less than 8
        //? SWEEP UNIT; target period is greater than $7FF

    }
    //? Triangle
    else if (channel_num == 2) {
        //? Linear counter == 0
    }
    //? Noise
    else if (channel_num == 3) {

    }
    //? Sampler
    else if (channel_num == 4) {
        //! DMC Channel always outputs value of its counter (regardless of enable)
        //! Enable bit only disables the automatic playback of sample
    }
    //? Invalid channel number
    else {
        cout << "WARNING (in APU::IsChannelMuted): illegal value for channel_num - [" << channel_num << "]. Continuing...\n";
        return false;
    }
}

vector<bool> APU::ChannelMuteStatuses() {
    vector<bool> mutes(5, false);
    for (int i = 0; i < 5; i++) {
        mutes[i] = IsChannelMuted(i);
    }
    return mutes;
}

uint16_t APU::FreqToRawPeriod(float fhz) {
    return (int)((111860.8f / fhz) - 1.0f);
}

//? General APU functionality
void APU::CopyData_OAM_DMA() {
    //TODO
}

void APU::ChannelEnable() {
    //TODO
}

//? Methods relevant to multiple channels
void APU::SQ_NOISE_SetVolume(uint8_t new_volume, uint8_t channel_num) {
    //? 0= Sq1, 1= Sw2, 2=Noise
    if (channel_num > 2) {
        cout << "WARNING (in APU::SQ_NOISE_SetVolume): illegal value for channel_num - [" << channel_num << "]. Continuing...\n";
        return;
    }

    //? Volume is 4-bit value [0, 15]
    if (new_volume > 15) {
        cout << "WARNING (in APU::SQ_NOISE_SetVolume): illegal value for new_volume - [" << new_volume << "]. Continuing...\n";
        return;
    }

    //? Set appropriate volume register
    if (channel_num == 0) {
        //TODO
    }
    else if (channel_num == 1) {
        //TODO
    }
    else if (channel_num == 2) {
        //TODO
    }
}

void APU::SQ_TRI_SetRawPeriod(uint16_t raw_period, uint8_t channel_num) {
    //? 0= Sq1, 1= Sw2, 2=Tri
    if (channel_num > 2) {
        cout << "WARNING (in APU::SQ_TRI_SetRawPeriod): illegal value for channel_num - [" << channel_num << "]. Continuing...\n";
        return;
    }

    SQ_TRI_SetPeriodLO((uint8_t)raw_period, channel_num);
    SQ_TRI_SetPeriodHI((uint8_t)(raw_period >> 8), channel_num);
}

void APU::SQ_TRI_SetPeriodLO(uint8_t period_low_8, uint8_t channel_num) {
    //? 0= Sq1, 1= Sw2, 2=Tri
    if (channel_num > 2) {
        cout << "WARNING (in APU::SQ_TRI_SetPeriodLO): illegal value for channel_num - [" << channel_num << "]. Continuing...\n";
        return;
    }

    //? Set appropriate PERIOD_LO register
    if (channel_num == 0) {
        //TODO
    }
    else if (channel_num == 1) {
        //TODO
    }
    else if (channel_num == 2) {
        //TODO
    }
}

void APU::SQ_TRI_SetPeriodHI(uint8_t period_high_8, uint8_t channel_num) {
    //? 0= Sq1, 1= Sw2, 2=Tri
    if (channel_num > 2) {
        cout << "WARNING (in APU::SQ_TRI_SetPeriodHI): illegal value for channel_num - [" << channel_num << "]. Continuing...\n";
        return;
    }

    //? Set appropriate PERIOD_HI register
    if (channel_num == 0) {
        //TODO
    }
    else if (channel_num == 1) {
        //TODO
    }
    else if (channel_num == 2) {
        //TODO
    }
}

void APU::SQ_TRI_NOISE_SetLenCntVal(uint8_t counter_val, uint8_t channel_num) {
    //? 0= Sq1, 1= Sw2, 2=Tri, 3=Noise
    if (channel_num > 3) {
        cout << "WARNING (in APU::SQ_TRI_NOISE_SetLenCntVal): illegal value for channel_num - [" << channel_num << "]. Continuing...\n";
        return;
    }

    //? Set appropriate LENGTH_CNT register
    if (channel_num == 0) {
        //TODO
    }
    else if (channel_num == 1) {
        //TODO
    }
    else if (channel_num == 2) {
        //TODO
    }
    else if (channel_num == 3) {
        //TODO
    }
}

//? Square/Pulse wave channels (2 channels)
void APU::SQ_SetDuty(float duty_cycle, uint8_t pulse_channel) {
    if (pulse_channel > 1) {
        cout << "WARNING (in APU::SQ_SetDuty): illegal value for Pulse channel - [" << pulse_channel << "]. Continuing...\n";
        return;
    }

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
        cout << "WARNING (in APU::SQ_SetDuty, Pulse # " << pulse_channel << "): illegal value for Duty Cycle - [" << pulse_channel << "]. Continuing...\n";
    }
}

void APU::SQ_SetSweep(uint8_t flags, uint8_t pulse_channel) {
    if (pulse_channel > 1) {
        cout << "WARNING (in APU::SQ_SetSweep): illegal value for Pulse channel - [" << pulse_channel << "]. Continuing...\n";
        return;
    }

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