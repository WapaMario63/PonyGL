#ifndef PONYGL_H
#define PONYGL_H

#include "pne_utils.h"
#include "Act.h"
#include <list>

class PonyGL
{
public:
    PonyGL();

    void run();

    std::list<Act> GetAllActsFromFolder(std::string directory);

private:
    GLFWwindow* window;
};

#endif // PONYGL_H
