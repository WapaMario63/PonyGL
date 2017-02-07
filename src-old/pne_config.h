#ifndef PNE_CONFIG
#define PNE_CONFIG

/*! @file pne_config.h

  \brief Contains compile configurations.

  You can choose to use either GLFW or Qt's OpenGL Implementations for OpenGL rendering.
  You can also choose to use either Raknet or Qt's Networking Libraries.
  You can ALSO choose if you want to use the standard C++ libraries or Qt's Thread-safe Implementations

  Vulkan define is for when the Vulkan API actually comes out (and I am damn excited for it).

*/

#define GLEW_STATIC

#if defined _WIN32 || defined WIN32
#include <windows.h>
#endif

#define PONENGINE_VERSION "v0.0.1a (SDL2)"
#define PONENGINE_DEFAULT_RESOLUTION_X 800
#define PONENGINE_DEFAULT_RESOLUTION_Y 600

#define PONENGINE_BULLET_PHYSICS
#define PONENGINE_GLM_VECMAT
#define PONENGINE_SFML_AUDIO

#include <SFML/Main.hpp>

/* OpenGL Libraries and Rendering Includes */
#if defined PONENGINE_QT_OPENGL
#include <QtCore>
#include <QtCore/QCoreApplication>
#include <QtOpenGL>
#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#elif defined PONENGINE_GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif defined PONENGINE_SDL2
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined PONENGINE_VULKAN
    /* Implement Vulkan when the API comes out. */
#endif

/* Input Library Includes */
#if defined PONENGINE_QT_INPUT
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#elif defined PONENGINE_OIS
#include <OIS.h> /* May not be the actual header */
#endif

/* Networking Library Includes */
#if defined PONENGINE_QT_NETWORK
#include <QtNetwork>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#elif defined PONENGINE_RAKNET
    /* Raknet includes go here */
#endif

/* Physics Library Includes */
#if defined PONENGINE_BULLET_PHYSICS
/*#include <btBulletCollisionCommon.h> Some File not found errors related to bullet
#include <btBulletDynamicsCommon.h> Commeting this until it's fixed */
#elif defined PONENGINE_PHYSX_PHYSICS
#include <phsyx3.h> /* May not be the actual header */
#elif defined PONENGINE_HAVOK_PHYSICS
#include <havok.h> /* May not be the actual header */
#endif

/* GUI Library includes */
#if defined PONENGINE_IMGUI
#include "Third_Party/Imgui/imgui.h"
#include "Third_Party/Imgui/imconfig.h"
#elif defined PONENGINE_ROCKET_GUI
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#elif defined PONENGINE_CEGUI
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <CEGUI/Base.h>
#elif defined PONENGINE_MYGUI
#include <MyGUI.h>
#include <MyGUI_OpenGLPlatform.h>
#endif

/* Sound Library Includes */
#if defined PONENGINE_OPENAL
#include <al.h>
#include <alc.h>
#elif defined PONENGINE_SFML_AUDIO
#include <SFML/Audio.hpp>
#elif defined PONENGINE_FMOD
#include <fmod.h> /* may not be the actual header */
#endif


/* Misc Library Includes */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

/* Use either the standard libraries or Qt's Thread-safe implementations */
#ifdef PONENGINE_QT_FRAMEWORK
#include <QtCore>
#include <QString>
#include <QFile>
#include <QDir>
#include <QList>
#include <QVector>
#include <QException>
#include <QPointer>
#include <QTextStream>
#include <QObject>
#include <QtMath>
#else
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <exception>
#include <map>
#include <deque>
#include <set>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#endif


#ifdef PONENGINE_QT_VECMAT
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QVectorIterator>
#include <QMatrix>
#include <QMatrix2x2>
#include <QMatrix3x3>
#include <QMatrix4x4>
#elif defined PONENGINE_GLM_VECMAT
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#endif /* PNE_CONFIG */

