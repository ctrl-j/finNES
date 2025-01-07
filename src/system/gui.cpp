#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>

#include <iostream>
#include <vector>

#include "../../include/display.hh"
#include "../../include/gui.h"
#include "../../include/nes.hh"
#include "../../include/tests.hh"

using namespace std;

Display* DISPLAY;

const char* IST_Strings[7] = {
    "SUCCESS",
    "ERROR",
    "WARNING",
    "NULL_PTR",
    "INVALID_PARAM",
    "READ_ERROR",
    "WRITE_ERROR"
};

int main(int argc, char** argv) {
    DISPLAY = new Display();

    DISPLAY->gui->wins->end();
    DISPLAY->gui->wins->show(argc, argv);
    
    return Fl::run();
}

GUI::GUI() {
    wins = new Fl_Double_Window(800, 600, "finNES v0.2");
    msg_box = new Fl_Box(20, 40, 300, 100, "Fuck her right\nin the pussy");
    
    msg_box->box(FL_UP_BOX);
    msg_box->labelfont(FL_BOLD+FL_ITALIC);
    msg_box->labelsize(36);
    msg_box->labeltype(FL_SHADOW_LABEL);

    // #ifdef TEST
    //     //? Run appropriate test stuff
    //     test_main();
    // #endif

    // VM = new NES();
    
    // //? Init vertex data for the starting screen
    // vector<int> x_tri_1 = {200, 300, 500};
    // vector<int> y_tri_1 = {200, 200, 100};

    // vector<int> x_tri_2 = {400, 600, 500};
    // vector<int> y_tri_2 = {200, 200, 100};

    // vector<float> tri1 = VM->_VID_OUT->PxToNDC_Tri(x_tri_1, y_tri_1);
    // vector<float> tri2 = VM->_VID_OUT->PxToNDC_Tri(x_tri_2, y_tri_2);

    // VM->_VID_OUT->verts = new float[18];

    // int GUI_SCENE_STATUS = VM->_VID_OUT->Scene();
    // cout << "GUI SCENE EXIT CODE: " << GUI_SCENE_STATUS << "\n";
}

GUI::~GUI() {
    delete VM->_VID_OUT->verts;
    delete VM;
}