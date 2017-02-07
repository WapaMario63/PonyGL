#ifndef RENDERER_H
#define RENDERER_H

#include <pne_utils.h>
#include <assimp/scene.h>
#include <list>
#include "GLSLShader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    std::string type;
    aiString path;
};

class Renderer
{
public:
    Renderer();

    void start();

    void renderAct();

    void stop();

private:
    std::list<GLSLShader> shaders;
};

#endif // RENDERER_H
