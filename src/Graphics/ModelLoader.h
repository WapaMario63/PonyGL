#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "pne_utils.h"
#include "Mesh.h"
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

class ModelLoader
{
public:
    ModelLoader(const char *path);
    void Draw(GLSLShader shader);
private:
    std::vector<Texture> loadedTextures;
    std::vector<Mesh> _meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterials(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif // MODELLOADER_H
