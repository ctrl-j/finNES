//! GLAD packaged for OpenGL version 4.0
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>
#include "../include/display.hh"



using namespace std;

Display::Display() {
    
}

Display::~Display() {

}

int Display::Init() {
    //? Initialize GLFW
    glfwInit();
    //? Tell GLFW to use v4.0 of OpenGL API
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //? Tell GLFW to use the "Core" profile (smaller subset of features w/o backwards compat.)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //? Creates 640x480 windowed mode window, sets it in context
    monitor = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(640, 480, "finNES v0.1", NULL, NULL);
    if (window == NULL) {
        cout << "*** GLFW ERROR: Failed to create GLFW window. Exiting... ***\n";
        return -1;
    }
    glfwMakeContextCurrent(window);

    //? Initialize GLAD function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "*** GLAD ERROR: Failed to initialize GLAD. Exiting...\n";
        return -1;
    } 

    //? Tell GLFW what to show in the viewport
    glViewport(0, 0, 640, 480);

    //? Set GLFW callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    return 0;
}

int Display::Scene() {
    //? Main window loop
    while (!glfwWindowShouldClose(window))
    {
        //Render();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void Display::Render() {

}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    //? Set the viewport to the new window size
    glViewport(0, 0, w, h);
}

void window_close_callback(GLFWwindow* window)
{
    //? This can be used to filter out unwanted close requests
    //if (!time_to_close)
    //    glfwSetWindowShouldClose(window, GLFW_FALSE);
}

void window_size_callback(GLFWwindow* window, int w, int h)
{

}
