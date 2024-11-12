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

    //? Init vertex data for the starting screen
    vector<int> x_tri_1 = {200, 300, 500};
    vector<int> y_tri_1 = {200, 200, 100};

    vector<int> x_tri_2 = {400, 600, 500};
    vector<int> y_tri_2 = {200, 200, 100};

    vector<float> tri1 = mainDisplay.PxToNDC_Tri(x_tri_1, y_tri_1);
    vector<float> tri2 = mainDisplay.PxToNDC_Tri(x_tri_2, y_tri_2);

    mainDisplay.verts = new float[18];

    int GUI_SCENE_STATUS = mainDisplay.Scene();
    cout << "GUI SCENE EXIT CODE: " << GUI_SCENE_STATUS << "\n";

    delete mainDisplay.verts;
    
    //? Start the virtual machine

}

