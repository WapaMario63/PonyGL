#include "Font.h"
#include <iostream>

Font::Font(const char *fontFile)
{
    if (FT_Init_FreeType(&_ft))
        std::cout << "Error: Cannot load Freetype \nError Code: PNE_GFX_ERR::FREETYPE_INIT_FAILURE" << std::endl;

    if (FT_New_Face(_ft, fontFile,0, &_face))
        std::cout << "Error: Cannot load font: " << fontFile << "\nError Code: PNE_GFX_ERR::FONT_NOT_FOUND" << std::endl;

    FT_Set_Pixel_Sizes(_face,0,48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
        {
            std::cout << "Error: Cannot load glyph from font \nError Code: PNE_GFX_ERR::FONT_BAD_GLYPH" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RED,
            static_cast<GLsizei>(_face->glyph->bitmap.width),
            static_cast<GLsizei>(_face->glyph->bitmap.rows),
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            _face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FCharacter character =
        {
            texture,
            glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
            glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
            static_cast<GLuint>(_face->glyph->advance.x)
        };

        _characters.insert(std::pair<char, FCharacter>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(_face);
    FT_Done_FreeType(_ft);

    glGenVertexArrays(1, &fVAO);
    glGenBuffers(1, &fVBO);
    glBindVertexArray(fVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::RenderText(GLSLShader shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    shader.load();
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(fVAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        FCharacter ch = _characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0,0.0 },
            { xpos,     ypos,       0.0,1.0 },
            { xpos + w, ypos,       1.0,1.0 },

            { xpos,     ypos + h,   0.0,0.0 },
            { xpos + w, ypos,       1.0,1.0 },
            { xpos + w, ypos + h,   1.0,0.0 }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, fVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
