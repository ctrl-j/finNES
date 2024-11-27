#include <iostream>
#include <cstdint>
#include <vector>

//! This file will define what is common between the APU and CPU
#include "../../include/ricoh_2a03.hh"

using namespace std;

/* ************************************
* Constructors and Destructors          *
* for Ricoh class core, APU, and CPU    *
* ************************************* */
Ricoh_2A03::Ricoh_2A03(bool derived_caller) {
    if (derived_caller == false) {
        //? Init core object members
        cpu = new CPU();
        apu = new APU();
        // BASE_CPU_PTR = cpu;
        // BASE_APU_PTR = apu;
    }
    // else {
    //     // Attach global obj pointers to base class for any derived classes
    //     if (BASE_CPU_PTR != NULL) {
    //         cpu = BASE_CPU_PTR;
    //     }
    //     else {
    //         cout << "ERROR (in Ricoh_2A03 constructor): BASE_CPU_PTR is NULL; exiting...\n";
    //         cpu = NULL;
    //         apu = NULL;
    //         return;
    //     }
    //     if (BASE_APU_PTR != NULL) {
    //         apu = BASE_APU_PTR;
    //     }
    //     else {
    //         cout << "ERROR (in Ricoh_2A03 constructor): BASE_APU_PTR is NULL; exiting...\n";
    //         cpu = NULL;
    //         apu = NULL;
    //         return;
    //     }
    // }
}

Ricoh_2A03::~Ricoh_2A03() {
    //? Destroy core object members
    delete cpu;
    delete apu;
}

/* *************************
* Ricoh class core routines *
* **************************/
