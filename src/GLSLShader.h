#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include "include_graphics_libs.h"

class GLSLShader
{
public:
    GLSLShader();
    GLSLShader(const char* vertF, const char* fragF);

    GLuint GetProgram();
    char* GetErrorString();

    void SetProgram(GLuint program);
    void load();

private:
    GLuint _program;

    char* errorString;
};

#endif // GLSLSHADER_H
