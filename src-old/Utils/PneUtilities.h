#ifndef PNEUTILITIES_H
#define PNEUTILITIES_H

#include <pne_config.h>

class PneUtilities
{
public:
    PneUtilities();

    // check if the computer is little endian or big endian
    bool IsBigEndian();
    int CharToInt(char* buffer, int len);
    //sf::Vector3f toSFMLvector3f(glm::vec3 vect);
    //sf::Vector2f toSFMLvector2f(glm::vec2 vect);
    //glm::vec3 toGLMvector3(sf::Vector3f vect);
    //glm::vec2 toGLMvector2(sf::Vector2f vect);
};

#endif // PNEUTILITIES_H
