#ifndef PONYGL_H
#define PONYGL_H

#if defined(WIN32) || defined(__WIN32)
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class PonyGL
{
public:
    // Constructor/Destructor
    PonyGL();
    ~PonyGL();

    // Global Vars
    GLFWwindow* window;
    const GLuint winWidth = 1280;
    const GLuint winHeight = 720;

    // GET Functions

    // Helper Functions

    // Main Functions
    void initializeGL();
    void drawGL();
};

// Global extern from main

#endif // PONYGL_H
