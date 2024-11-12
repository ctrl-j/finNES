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

//? Square/Pulse wave channels (2 channels)