#include <iostream>
#include <cstdint>

#include "../include/display.hh"
//#include "../include/ricoh_2a03.hh"
//#include "../include/ppu.hh"

using namespace std;

//! Called when window is queued to close


int main(int argc, char** argv) {
    //? Init memory

    //? Load "cartridge" into ROM

    //? Init Ricoh 2A03 (CPU+APU)

    //? Init PPU

    //? Init sound

    //? Init display GUI
    Display mainDisplay;
    int GUI_INIT_STATUS = mainDisplay.Init();
    cout << "GUI INIT EXIT CODE: " << GUI_INIT_STATUS << "\n";

    vector<int> xs = {400, 600, 500};
    vector<int> ys = {200, 200, 100};

    vector<float> v = mainDisplay.PxToNDC_Tri(xs, ys);
    mainDisplay.verts = &v[0];

    int GUI_SCENE_STATUS = mainDisplay.Scene();
    cout << "GUI SCENE EXIT CODE: " << GUI_SCENE_STATUS << "\n";
    
    //? Start the virtual machine

}

