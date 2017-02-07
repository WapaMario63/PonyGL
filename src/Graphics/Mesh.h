#ifndef MESH_H
#define MESH_H

#include "Renderer.h"
#include "GLSLShader.h"

class Mesh
{
public:

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> Vertices, std::vector<GLuint> Indices, std::vector<Texture> Textures);

    void Draw(GLSLShader shader);
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();

};

#endif // MESH_H
