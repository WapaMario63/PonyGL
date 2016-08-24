#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>
#include <Render/Shader.h>
#include <assimp/Importer.hpp>

#include <vector>
#include <string>


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

class Cube
{
public:
    GLuint texture;
    glm::vec3 position;
    glm::mat4 model;
    unsigned char* textureImage;
    int textureWidth, textureHeight;

    Cube();

    void SetPosition(glm::vec3 pos);
    void SetPosition(float posx, float posy, float posz);

    void SetTexture(unsigned char* image);
    void UseTexture(GLuint texture);
    void UseProgram(GLuint programId);

};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    void Draw(Shader shader);
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

#endif // MESH_H
