#pragma once
#include <Fl/Fl.H>
#include <vector>

#include "gui.h"

using namespace std;

class Display {
public:
    GUI* gui;
    int window_width;
    int window_height;
    unsigned int current_shader_program, current_vao, current_vbo;
    
    float* verts;
    int vertsN;

    Display();
    ~Display();

    // GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    int Scene();
    // void Render();
    // void ProcessInput();
    
    //? Converts pixel vertices to Normalized Device Coordinate vertices
    vector<vector<float>> PxToNDC_Rect(int x, int y, int w, int h);
    vector<vector<float>> PxToNDC_Tri(vector<int> v1, vector<int> v2, vector<int> v3);
    vector<float> PxToNDC_Tri(vector<int> x, vector<int> y);

};



//! !!!!!!!!!!!!!!!!!!!!!!!!!!
//! GLFW Window Callbacks
//! MUST BE ASSIGNED AT INIT
//! !!!!!!!!!!!!!!!!!!!!!!!!!!
//* Called when window is requested to close; useful for filtering unwanted close requests
// void window_close_callback(GLFWwindow*);
// //* Called whenever window is resized
// void window_size_callback(GLFWwindow* window, int w, int h);
// //* Called whenever window is resized, in order to handle changes in what is rendered
// void framebuffer_size_callback(GLFWwindow* window, int w, int h); 