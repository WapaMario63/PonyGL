#include "ModelLoader.h"
#include "pne_utils.h"
#include <iostream>
#include <SOIL2/SOIL2.h>

ModelLoader::ModelLoader(const char *path)
{
    this->loadModel(path);
}

void ModelLoader::Draw(GLSLShader shader)
{
    for (GLuint i=0; i<_meshes.size(); i++)
        _meshes[i].Draw(shader);
}

void ModelLoader::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* Scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    std::cout << "Importing model..." << std::endl;
    if (!Scene || Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cout << "Error reading common model file. Details: " << import.GetErrorString() << std::endl;
        return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));
    this->processNode(Scene->mRootNode, Scene);
}

void ModelLoader::processNode(aiNode* node, const aiScene *scene)
{

    for (GLuint i=0; i<node->mNumMeshes; i++)
    {
        std::cout << "Processing main nodes and mesh. Count: " << i << std::endl;
        aiMesh* Mesh = scene->mMeshes[node->mMeshes[i]];
        this->_meshes.push_back(this->processMesh(Mesh, scene));
    }
    for (GLuint i=0; i<node->mNumChildren; i++)
    {
        std::cout << "Processing child nodes and mesh. Count: " << i << std::endl;
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene)
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
            glm::vec2 texCoords;
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texCoords;
        }
        else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (GLuint i=0; i<mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j=0; j<face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex > 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = this->loadMaterials(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> speculiarMaps = this->loadMaterials(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), speculiarMaps.begin(), speculiarMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModelLoader::loadMaterials(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> Textures;
    for (GLuint i=0; i<mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        GLboolean skip = false;
        for (GLuint j=0; j<loadedTextures.size(); j++)
        {
            if (loadedTextures[j].path == str)
            {
                Textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;

            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            Textures.push_back(texture);
            this->loadedTextures.push_back(texture);

        }
    }
    return Textures;
}

GLuint TextureFromFile(const char* path, std::string directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    GLuint textureId;
    glGenTextures(1, &textureId);
    int width, height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureId;
}
