#ifndef INCLUDE_GRAPHICS_LIBS_H
#define INCLUDE_GRAPHICS_LIBS_H

#if defined WIN32 || defined _WIN32
#include <windows.h>
#endif

#ifdef USE_PONYVK
#include <vulkan.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#endif

#endif // INCLUDE_GRAPHICS_LIBS_H
