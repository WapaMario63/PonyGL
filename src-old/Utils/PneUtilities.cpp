#include "PneUtilities.h"

PneUtilities::PneUtilities()
{

}

bool PneUtilities::IsBigEndian()
{
    int a = 1;
    return !((char*)&a)[0];
}

int PneUtilities::CharToInt(char *buffer, int len)
{
    int final = 0;
    if (!IsBigEndian())
    {
        for (auto i=0; i<len; i++)
            ((char*)&final)[i] = buffer[i];
    }
    else
    {
        for (auto i=0; i<len; i++)
            ((char*)&final)[3-i] = buffer[i];
    }

    return final;
}

/*sf::Vector3f PneUtilities::toSFMLvector3f(glm::vec3 vect)
{
    return sf::Vector3f(vect.x,vect.y,vect.z);
}
sf::Vector2f PneUtilities::toSFMLvector2f(glm::vec2 vect)
{
    return sf::Vector2f(vect.x,vect.y);
}
glm::vec3 PneUtilities::toGLMvector3(sf::Vector3f vect)
{
    return glm::vec3(vect.x,vect.y,vect.z);
}
glm::vec2 PneUtilities::toGLMvector2(sf::Vector2f vect)
{
    return glm::vec2(vect.x,vect.y);
}*/
