#ifndef MODEL_H
#define MODEL_H

#include "Render/Shader.h"
#include "Render/Mesh.h"
#include <vector>
#include <string>

#include <assimp/scene.h>

GLint TextureFromFile(const char *path, std::string directory);

class Model
{
public:
    Model(char *path);
    void Draw(Shader shader);
private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string dir;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H
