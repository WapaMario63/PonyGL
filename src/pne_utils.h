#ifndef PNE_UTILS_H
#define PNE_UTILS_H

#include "include_graphics_libs.h"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DEFAULT_WINDOW_RESOLUTION_WIDTH 1280
#define DEFAULT_WINDOW_RESOLUTION_HEIGHT 720

GLuint TextureFromFile(const char *path, std::string directory);

#endif // PNE_UTILS_H
