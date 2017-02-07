#ifndef ACT_H
#define ACT_H

#include "pne_utils.h"
#include <list>
#include "Graphics/ModelLoader.h"
#include "Graphics/Light.h"

class Act
{
public:
    Act();



private:
    int id;
    std::string name;
    std::string gamemode;
    std::string description;
    GLuint _fbo;
    GLuint _rbo;
    std::string ActDir;

    // TODO: Entity Storage and loaders
    //e.g. models and entity vectors (std::vector<Terrain>, std::vector<Entitiy>, etc.)
    std::list<ModelLoader> models;
    std::list<Light> lights;

    GLuint generateAttachmentTexture(bool depth, bool stencil);
    void GenerateFramebuffers();

};

#endif // ACT_H
