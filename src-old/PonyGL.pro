#-------------------------------------------------
#
# Project created by QtCreator 2015-09-02T19:10:04
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PonyGL
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += E:/Software_Dev/Libraries/glfw-3.1.1/include/
INCLUDEPATH += E:/Software_Dev/Libraries/glew-1.13.0/include/
INCLUDEPATH += E:/Software_Dev/Libraries/assimp-3.1.1-win-binaries/include
INCLUDEPATH += E:/Software_Dev/Libraries/SFML-2.3.2/include
LIBS += -LE:/Software_Dev/Libraries/glfw-3.1.1/lib-mingw
LIBS += -LE:/Software_Dev/Libraries/glew-1.13.0/lib/Win32
LIBS += -LE:/Software_Dev/Libraries/SOIL2/lib/windows
LIBS += -LE:/Software_Dev/Libraries/assimp-3.1.1-win-binaries/lib
LIBS += -LE:/Software_Dev/Libraries/SFML-2.3.2/lib

LIBS *= -lglfw3 -lassimp -lgdi32 -lglew32 -lsoil2 -lopengl32
LIBS *= -lvorbis -lvorbisenc -lvorbisfile -lFLAC -logg -lsfml-main -lsfml-audio -lsfml-audio-s

SOURCES += main.cpp \
    PonyGL.cpp \
    Camera.cpp \
    Render/Shader.cpp \
    Render/Cube.cpp \
    Render/Mesh.cpp \
    Render/Model.cpp \
    Render/Light.cpp \
    Sound/PneSound.cpp \
    Utils/PneUtilities.cpp

HEADERS  += \
    PonyGL.h \
    Camera.h \
    Render/Shader.h \
    Render/Mesh.h \
    Render/Model.h \
    Render/Light.h \
    Sound/PneSound.h \
    Utils/PneUtilities.h \
    pne_config.h

FORMS    +=

DISTFILES += \
    TriangleVertexShader.pvs \
    TriangleFragmentShader.pfs
