#include "Act.h"
#include <QtXml/QXmlInputSource>
#include <QDir>
#include "PonyGL.h"
#include <sstream>

Act::Act()
{

}

void Act::GenerateFramebuffers()
{
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

}

GLuint Act::generateAttachmentTexture(bool depth, bool stencil)
{
    GLenum attachment_type = NULL;
    if (!depth && !stencil)
        attachment_type = GL_RGB;
    else if (depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if (!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)attachment_type, DEFAULT_WINDOW_RESOLUTION_WIDTH, DEFAULT_WINDOW_RESOLUTION_HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, DEFAULT_WINDOW_RESOLUTION_WIDTH, DEFAULT_WINDOW_RESOLUTION_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

std::list<Act> PonyGL::GetAllActsFromFolder(std::string directory)
{
    std::list<Act> actList;



}
