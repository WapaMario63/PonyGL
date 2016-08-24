#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    GLuint Program;
public:
    Shader();
    Shader(const GLchar* vertexFile, const GLchar* fragmentFile);

    GLuint useProgram(GLuint program);
    void use();
};

#endif // SHADER_H
