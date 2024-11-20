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