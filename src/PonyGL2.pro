QT += core

INCLUDEPATH += E:\Software_Dev\Libraries\glew-2.0.0\include
INCLUDEPATH += E:\Software_Dev\Libraries\glfw-3.2.1.bin.WIN32\include
INCLUDEPATH += E:\Software_Dev\Libraries\SFML-2.4.1\include
INCLUDEPATH += E:\Software_Dev\Libraries\freetype-2.6\include
INCLUDEPATH += E:\Software_Dev\Libraries\glm
INCLUDEPATH += E:\Software_Dev\Libraries\assimp-3.3.1\include
INCLUDEPATH += E:\Software_Dev\Libraries\SOIL2\src

LIBS += -LE:\Software_Dev\Libraries\glew-1.13.0\lib\Win32
LIBS += -LE:\Software_Dev\Libraries\glfw-3.2.1.bin.WIN32\lib-mingw
LIBS += -LE:\Software_Dev\Libraries\SFML-2.4.1\lib
LIBS += -LE:\Software_Dev\Libraries\freetype-2.6\objs
LIBS += -LE:\Software_Dev\Libraries\assimp-3.3.1\build
LIBS += -LE:\Software_Dev\Libraries\SOIL2\lib\windows

LIBS *= -lglew32 -lglfw3 -lsfml-system -lsfml-audio -lsfml-graphics -lassimp.dll -lfreetype -lsoil2 -lopengl32 -lglu32 -lgdi32

CONFIG += c++11
CONFIG += console

SOURCES += \
    main.cpp \
    PonyGL.cpp \
    GLSLShader.cpp \
    Graphics/Font.cpp \
    Graphics/ModelLoader.cpp \
    Graphics/Renderer.cpp \
    Graphics/Mesh.cpp \
    Act.cpp \
    Graphics/Camera.cpp \
    Graphics/Light.cpp \
    Sound/Sound.cpp

HEADERS += \
    PonyGL.h \
    include_graphics_libs.h \
    pne_utils.h \
    GLSLShader.h \
    Graphics/Font.h \
    Graphics/ModelLoader.h \
    Graphics/Renderer.h \
    Graphics/Mesh.h \
    Act.h \
    Graphics/Camera.h \
    Graphics/Light.h \
    Sound/Sound.h

DISTFILES += \
    Graphics/triangle.vert \
    Graphics/triangle.frag \
    Graphics/text.fsh \
    Graphics/text.vsh \
    Graphics/lightBox.fsh \
    Graphics/lightBox.vsh \
    Graphics/RDash.vsh \
    Graphics/RDash.fsh \
    Graphics/simpleDepth.vsh \
    Graphics/simpleDepth.fsh \
    Graphics/Screen.vsh \
    Graphics/Screen.fsh
