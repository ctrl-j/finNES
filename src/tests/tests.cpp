#include <iostream>
#include <vector>

#include "../../include/tests.hh"
#include "../../include/nes.hh"

using namespace std;

int test_main() {

    VM = new NES();
    
    //? Init vertex data for the starting screen
    float v[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        0.4222f, 1.0f, 0.0f
    };

    VM->_VID_OUT->verts = v;
    VM->_VID_OUT->vertsN = 3;

    int GUI_SCENE_STATUS = VM->_VID_OUT->Scene();
    cout << "GUI SCENE EXIT CODE: " << GUI_SCENE_STATUS << "\n";

    delete VM->_VID_OUT->verts;
    
    delete VM;

    return EXIT_SUCCESS;
}