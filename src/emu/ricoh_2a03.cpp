//! This file will define what is common between the APU and CPU

#include "../../include/ricoh_2a03.hh"

/* ************************************
* Constructors and Destructors          *
* for Ricoh class core, APU, and CPU    *
* ************************************* */

Ricoh_2A03::Ricoh_2A03() {
    //? Init core object members
    cpu = new CPU();
    apu = new APU();
}

Ricoh_2A03::~Ricoh_2A03() {
    //? Destroy core object members
    delete cpu;
    delete apu;
}

APU::APU() {
    //? Init APU object members
    MXR = new Mixer();
    FMCNT = new FrameCounter();
}
APU::~APU(){
    //? Destroy APU object members
    delete MXR;
    delete FMCNT;
}

CPU::CPU() {

}
CPU::~CPU(){
    
}

/* *************************
* Ricoh class core routines *
* **************************/
