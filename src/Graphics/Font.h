#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include <freetype.h>
#include "pne_utils.h"
#include "GLSLShader.h"
#include <map>
#include <string>

struct FCharacter
{
    GLuint      textureId;
    glm::ivec2  size;
    glm::ivec2  bearing;
    GLuint      advance;
};

class Font
{
public:
    Font(const char* fontFile);

    void RenderText(GLSLShader shader, std::string text, float x, float y, float scale, glm::vec3 color);

private:
    std::map<char, FCharacter> _characters;
    FT_Library _ft;
    FT_Face _face;

    GLuint fVAO, fVBO;
};

#endif // FONT_H
