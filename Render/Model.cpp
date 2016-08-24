#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include <iostream>

Model::Model(char *path)
{
    this->loadModel(path);
}

void Model::Draw(Shader shader)
{
    for (GLuint i=0; i<this->meshes.size(); i++)
        this->meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* Scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!Scene || Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cout << "Error reading Model! Info: " << import.GetErrorString() << std::endl;
        return;
    }

    this->dir = path.substr(0, path.find_last_of('/'));
    this->processNode(Scene->mRootNode, Scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (GLuint i=0; i<node->mNumMeshes; i++)
    {
        aiMesh* Mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(Mesh, scene));
    }
    for (GLuint i=0; i<node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (GLuint i=0; i<mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 verts;
        verts.x = mesh->mVertices[i].x;
        verts.y = mesh->mVertices[i].y;
        verts.z = mesh->mVertices[i].z;
        vertex.Position = verts;

        glm::vec3 norms;
        norms.x = mesh->mNormals[i].x;
        norms.y = mesh->mNormals[i].y;
        norms.z = mesh->mNormals[i].z;
        vertex.Normal = norms;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texCoord;
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texCoord;
        } else vertex.TexCoords = glm::vec2(0.0f,0.0f);

        vertices.push_back(vertex);
    }

    for (GLuint i=0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j=0; j<face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> speculiarMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_speculiar");
        textures.insert(textures.end(), speculiarMaps.begin(), speculiarMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> Textures;
    for (GLuint i=0; i<mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        GLboolean skip = false;
        for (GLuint j=0; j<textures_loaded.size(); j++)
        {
            if (textures_loaded[j].path == str)
            {
                Textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture mTexture;
            mTexture.id = TextureFromFile(str.C_Str(), this->dir);
            mTexture.type = typeName;
            mTexture.path = str;
            Textures.push_back(mTexture);
            this->textures_loaded.push_back(mTexture);
        }
    }
    return Textures;
}

GLint TextureFromFile(const char *path, std::string directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    GLuint textureId;
    glGenTextures(1, &textureId);

    int imgWidth, imgHeight;
    unsigned char* image = SOIL_load_image(filename.c_str(), &imgWidth, &imgHeight, 0, SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureId;
}
