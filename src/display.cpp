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
    glfwGetWindowSize(window, &window_width, &window_height);


    //? Initialize GLAD function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "*** GLAD ERROR: Failed to initialize GLAD. Exiting...\n";
        return -1;
    } 

    //? Tell GLFW what to show in the viewport
    glViewport(0, 0, window_width * 2, window_height * 2);

    //? Set GLFW callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    return 0;
}

int Display::Scene() {
    int success;
    unsigned int vertexShader, fragmentShader, shaderProgram;
    unsigned int VBO, VAO;
    char infoLog[512];
    const char *vertexShaderSource = "#version 400 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    //? Generate Vertex Buffer Object, Vertex Array Object to store vertex attributes
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    current_vao = VAO;
    current_vbo = VBO;

    // float vertices[] = {
    //     -1.0f, -0.5f, 0.0f,
    //     1.0f, -0.5f, 0.0f,
    //     0.0f,  0.5f, 0.0f
    // };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, verts, GL_STATIC_DRAW);
    //? Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //? Compilie the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);  
    //! Check if VERTEX SHADER compiled successfully
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //? Compile the fragment (pixel) shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //! Check if FRAGMENT SHADER compiled successfully
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //? Create shader program, linking the two shader objects
    shaderProgram = glCreateProgram();
    current_shader_program = shaderProgram;
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //! Check if shader program linked successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    //? Delete old shader objects now that they have been linked in a program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 


    //? Main window loop
    while (!glfwWindowShouldClose(window))
    {
        //? Input commands
        ProcessInput();

        //? Rendering commands
        Render();
    
        //? GLFW commands
        glfwPollEvents();           //? Check and call GLFW events
        glfwSwapBuffers(window);    //? Swap window framebuffer
    }

    glfwTerminate();
    return 0;
}

void Display::ProcessInput() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Display::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(current_shader_program);
    glBindVertexArray(current_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

vector<vector<float>> Display::PxToNDC_Rect(int x, int y, int w, int h) {
    //? Creates 3x4 vector for rectangle vertices
    vector<vector<float>> recVerts(4, vector<float>(3));

    //? Scale x, y, w, h to NDC range [-1, 1]
    float NDC_x = ((x / window_width) * 2) - 1;
    float NDC_y = ((y / window_height) * 2) - 1;
    float NDC_w = ((w / window_width) * 2) - 1;
    float NDC_h = ((h / window_height) * 2) - 1;

    //? Assign vector members for each vertex
    //? Top left vertex first, and then CCW around the rest
    recVerts[0] = {NDC_x, NDC_y, 0.0f};
    recVerts[1] = {NDC_x, NDC_y + NDC_h, 0.0f};
    recVerts[2] = {NDC_x + NDC_w, NDC_y + NDC_h, 0.0f};
    recVerts[3] = {NDC_x + NDC_w, NDC_y, 0.0f};

    return recVerts;
}

vector<vector<float>> Display::PxToNDC_Tri(vector<int> v1, vector<int> v2, vector<int> v3) {
    //? Creates 3x3 vector for triangle vertices
    vector<vector<float>> triVerts(3, vector<float>(3));

    return triVerts;
}

vector<float> Display::PxToNDC_Tri(vector<int> x, vector<int> y) {
    //? Creates 3x3 vector for triangle vertices
    vector<float> triVerts(9, 0.0f);

    //? Scale each x, y value
    for (int i = 0; i < 3; i++) {
        triVerts[(3*i)] = (((float)x[i] / window_width) * 2) - 1;
        triVerts[(3*i)+1] = (((float)y[i] / window_height) * 2) - 1;
        triVerts[(3*i)+2] = 0.0f;
    }

    return triVerts;
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
