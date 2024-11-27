#include <iostream>
#include <fstream>
#include <cstdint>

#include "../../include/nes.hh"
#include "../../include/tests.hh"

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
    #ifdef TEST
        //? Run appropriate test stuff
        test_main();
    #endif


    VM = new NES();
    
    //? Init vertex data for the starting screen
    vector<int> x_tri_1 = {200, 300, 500};
    vector<int> y_tri_1 = {200, 200, 100};

    vector<int> x_tri_2 = {400, 600, 500};
    vector<int> y_tri_2 = {200, 200, 100};

    vector<float> tri1 = VM->_VID_OUT->PxToNDC_Tri(x_tri_1, y_tri_1);
    vector<float> tri2 = VM->_VID_OUT->PxToNDC_Tri(x_tri_2, y_tri_2);

    VM->_VID_OUT->verts = new float[18];

    int GUI_SCENE_STATUS = VM->_VID_OUT->Scene();
    cout << "GUI SCENE EXIT CODE: " << GUI_SCENE_STATUS << "\n";

    delete VM->_VID_OUT->verts;
    
    delete VM;

    return EXIT_SUCCESS;
}