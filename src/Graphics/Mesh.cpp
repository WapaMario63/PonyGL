#include "Mesh.h"
#include <sstream>

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<GLuint> Indices, std::vector<Texture> Textures)
{
    this->vertices = Vertices;
    this->indices = Indices;
    this->textures = Textures;

    this->setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(GLSLShader shader)
{
    GLuint diffuseNr = 1;
    GLuint speculiarNr = 1;

    for (GLuint i=0; i<this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::stringstream ss;
        std::string num;
        std::string name = this->textures[i].type;

        if (name == "texture_diffuse") ss << diffuseNr++;
        else if(name == "texture_speculiar") ss << speculiarNr++;

        num = ss.str();

        glUniform1f(glGetUniformLocation(shader.GetProgram(), (name+num).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }

    glUniform1f(glGetUniformLocation(shader.GetProgram(), "material.shininess"), 16.0f);

    // Draw
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (GLuint i=0; i<this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
