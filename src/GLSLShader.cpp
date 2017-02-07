#include "GLSLShader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

GLSLShader::GLSLShader()
{

}

GLSLShader::GLSLShader(const char* vertF, const char* fragF)
{
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.exceptions(std::ifstream::badbit);
    fragFile.exceptions(std::ifstream::badbit);

    try
    {
        vertFile.open(vertF);
        fragFile.open(fragF);

        std::stringstream vertStream, fragStream;
        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        vertCode = vertStream.str();
        fragCode = fragStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "PNE_PGL_ERR::CANNOT_READ_SHADER" << std::endl;
    }

    const char* vShader = vertCode.c_str();
    const char* fShader = fragCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShader, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        errorString = infoLog;
        std::cerr << "Failed to compile GLSL vertex shader. \nError Code: PNE_PGL_ERR::FAILED_GL_VERT_SHADER_COMPILATION | Details:\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShader, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        errorString = infoLog;
        std::cerr << "Failed to compile GLSL fragment shader. \nError Code: PNE_PGL_ERR::FAILED_GL_FRAG_SHADER_COMPILATION | Details:\n" << infoLog << std::endl;
    }

    this->_program = glCreateProgram();
    glAttachShader(this->_program, vertex);
    glAttachShader(this->_program, fragment);
    glLinkProgram(this->_program);
    glGetProgramiv(this->_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->_program, 512, NULL, infoLog);
        errorString = infoLog;
        std::cout << "Failed to link GLSL shaders. \nError Code: PNE_PGL_ERR::FAILED_GL_SHADER_LINK | Details:\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

GLuint GLSLShader::GetProgram()
{
    return _program;
}

char* GLSLShader::GetErrorString()
{
    return errorString;
}

void GLSLShader::SetProgram(GLuint program)
{
    this->_program = program;
}

void GLSLShader::load()
{
    glUseProgram(_program);
}
