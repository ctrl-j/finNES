#pragma once
#include <GLFW/glfw3.h>

class Display {
public:
    GLFWmonitor* monitor;
    GLFWwindow* window;

    Display();
    ~Display();

    int Init();
    int Scene();
    void Render();

    

};

//! !!!!!!!!!!!!!!!!!!!!!!!!!!
//! GLFW Window Callbacks
//! MUST BE ASSIGNED AT INIT
//! !!!!!!!!!!!!!!!!!!!!!!!!!!
//* Called when window is requested to close; useful for filtering unwanted close requests
void window_close_callback(GLFWwindow*);
//* Called whenever window is resized
void window_size_callback(GLFWwindow* window, int w, int h);
//* Called whenever window is resized, in order to handle changes in what is rendered
void framebuffer_size_callback(GLFWwindow* window, int w, int h); 