#include "PonyGL.h"
#include "GLSLShader.h"
#include "Graphics/Font.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <iostream>
#include "Graphics/ModelLoader.h"
#include "Graphics/Camera.h"
#include <SOIL2/SOIL2.h>
#include "Graphics/Light.h"
#include "Sound/Sound.h"
#include <SFML/Audio.hpp>

bool keys[1024];
bool firstmouse=true;
bool freezeCam=false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = 400;
GLfloat lastY = 300;

int winWidth = 1280;
int winHeight = 720;

Camera camera(glm::vec3(0.0f,0.0f,0.5f));

const GLuint SHADOW_RES_X = 1024, SHADOW_RES_Y = 1024;

void RenderPlane(GLuint VAO, glm::mat4 model, glm::vec3 position, glm::vec3 scale, GLSLShader mainshader, GLuint diffTex, GLuint specTex);
void RenderCubes(GLuint VAO, glm::mat4 model, float angle, glm::vec3 rotation, GLSLShader mainShader, GLuint diffTex, GLuint specTex, GLuint amount, glm::vec3 cubePositions[]);
GLuint GetTextureFromFile(std::string filepath, bool sRGB);
GLuint generateAttachmentTexture(bool depth, bool stencil);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstmouse)
    {
        lastX = (GLfloat)xpos;
        lastY = (GLfloat)ypos;
        firstmouse = false;
    }

    GLfloat xOffset = (GLfloat)xpos - lastX;
    GLfloat yOffset = lastY -(GLfloat) ypos;
    lastX = (GLfloat)xpos;
    lastY = (GLfloat)ypos;

    if (!freezeCam)
        camera.ProcessMouseMoveMent(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((GLfloat)yoffset);
}

void do_movement(GLFWwindow* win)
{
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(Camera_Info::FORWARDS, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(Camera_Info::BACKWARDS, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(Camera_Info::LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(Camera_Info::RIGHT, deltaTime);
    if (keys[GLFW_KEY_SPACE]) camera.ProcessKeyboard(Camera_Info::UPWARDS, deltaTime);
    if (keys[GLFW_KEY_LEFT_SHIFT]) camera.ProcessKeyboard(Camera_Info::DOWNWARDS, deltaTime);

    if (keys[GLFW_KEY_R] && GLFW_PRESS) camera.movementSpeed = 0.5f;
    else if (keys[GLFW_KEY_LEFT_CONTROL] && GLFW_PRESS) camera.movementSpeed = 14.0f;
    else camera.movementSpeed = Camera_Info::SPEED;

    if (keys[GLFW_KEY_F1] && GLFW_PRESS)
    {
        freezeCam = true;
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (keys[GLFW_KEY_F2] && GLFW_PRESS)
    {
        freezeCam = false;
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

// ---------------------------------------------------------------------------
//      Actual OpenGL shit
// ---------------------------------------------------------------------------
PonyGL::PonyGL()
{

    std::cout << "Initializing display..." << std::endl;
    glfwInit();
    window = glfwCreateWindow(winWidth, winHeight, "PonyGL", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failed to create window\n" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create window for render.");
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        throw std::runtime_error("Failed to initialize GLEW.");
    }

    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Force OpenGL 4.3,
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // despite what the driver suggests
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Ditch the old OpenGL system (fixed-pipeline)
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    std::cout << "" << std::endl;
    std::cout << "Graphics Driver: OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Graphics Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Graphics Device: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "" << std::endl;
}

void PonyGL::run()
{
    glfwSetCursorPos(window, (double)winWidth/2,(double)winHeight/2);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::cout << "Loading Act UNDEFINED ..." << std::endl;
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };
    GLfloat planeVertices[] = {
            // Positions          // Normals         // Texture Coords
             8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 0.0f,
            -8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 5.0f,

             8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 0.0f,
            -8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 5.0f,
             8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 5.0f
        };
    GLfloat quadVertices[] = {
        // Positions   // TexCoords
         -1.0f,  1.0f,  0.0f, 1.0f,
         -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

         -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    GLuint VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    GLuint planeVAO, planeVBO;
    glGenBuffers(1, &planeVBO);
    glGenVertexArrays(1, &planeVAO);

    glBindVertexArray(planeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    GLuint quadVAO, quadVBO;
    glGenBuffers(1, &quadVBO);
    glGenVertexArrays(1, &quadVAO);

    glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    GLuint VAOl;
    glGenVertexArrays(1, &VAOl);

    glBindVertexArray(VAOl);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Shadow depth map
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RES_X, SHADOW_RES_Y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint texture = GetTextureFromFile("container2.png" , false);
    GLuint textureSpec = GetTextureFromFile("container2_specular.png", false);
    GLuint planeTex = GetTextureFromFile("basketball-floor-texture.jpg", true);
    GLuint planeTexSpec = GetTextureFromFile("basketball-floor-texture_spec.jpg", false);

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLuint texColorBuffer = generateAttachmentTexture(false, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winWidth, winHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Error: Framebuffer is incomplete" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    //glViewport(0,0, winWidth, winHeight);
    glEnable(GL_MULTISAMPLE);

    GLSLShader program("triangle.vert", "triangle.frag");
    GLSLShader* lightProg = new GLSLShader("lightBox.vsh", "lightBox.fsh");
    GLSLShader RDprog("RDash.vsh", "RDash.fsh");
    GLSLShader depthProg("simpleDepth.vsh", "simpleDepth.fsh");
    GLSLShader screenProg("Screen.vsh", "Screen.fsh");
    //Font* CRFont = new Font("CelestiaMediumRedux1.55.ttf");

    glUniform1i(glGetUniformLocation(program.GetProgram(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(program.GetProgram(), "shadowMap"), 1);

    ModelLoader* RDash = new ModelLoader("model/rdModelMat2.obj");

    Light light(program);
    light.setMaterialProperties(glm::vec3(0.02f, 0.02f, 0.02f),glm::vec3(0.5f, 0.4f, 0.2f), glm::vec3(1.0f, 0.8f, 0.6f));
    light.createSunLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f,1.0f,0.0f), 32);

    //Sound sound("WAPA_Turret.wav", Sound::snd_BGM);

    std::cout << "Loaded."<< std::endl;
    //sound.Play();

    int frametime = 0;

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        nbFrames++;

        if ((double)currentFrame - lastTime >= 1.0)
        {
            frametime = nbFrames;
            nbFrames = 0;
            lastTime += 1.0;
        }

        std::string wintitle = "PonyGL | FPS "+std::to_string(frametime)+"fps |  Pos: X: "+std::to_string(camera.position.x)+" Y: "+std::to_string(camera.position.y)+" Z: "+std::to_string(camera.position.z)+
                " | Rot: Pitch: "+std::to_string(camera.pitch)+" Yaw: "+std::to_string(camera.yaw);
        glfwSetWindowTitle(window, wintitle.c_str());

        do_movement(window);

        glm::vec3 pointLightPositions[] = {
            glm::vec3( -22.7f,  5.64f, -21.03f) * glm::sin((GLfloat)glfwGetTime()),
            glm::vec3( -17.89f, -1.72f, 7.69f) * glm::sin((GLfloat)glfwGetTime()),
            glm::vec3( 0.0f, 3.24f, 0.09f) * glm::sin((GLfloat)glfwGetTime()),
            glm::vec3( 17.33f, 1.86f, -14.73f) * glm::sin((GLfloat)glfwGetTime())
        };

        /*glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat nearPlane = 1.0f, farPlane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
        lightView = glm::lookAt(pointLightPositions[1], glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        depthProg.load();
        glUniformMatrix4fv(glGetUniformLocation(depthProg.GetProgram(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));*/
   //// DEPTH GENERATION


    //// END DEPTH GENERATION

    //// NORMAL RENDER
    /// Render framebuffer with normal stuff
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glViewport(0,0, winWidth, winHeight);
        //glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        //RDprog.load();
        GLint viewPosLoc = glGetUniformLocation(program.GetProgram(), "viewPos");
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.zoom, (GLfloat)winWidth / (GLfloat)winHeight, 0.1f, 100.0f);

        //RDprog.load();
        program.load();
        glUniformMatrix4fv(glGetUniformLocation(program.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(model, glm::vec3( 2.3f, -1.24f, -4.0f));
        model = glm::scale(model, glm::vec3(0.2f,0.2f,0.2f));
        //model = glm::rotate(model, 10.0f * glm::sin((GLfloat)glfwGetTime()), glm::vec3(1.0f, 0.0f,0.0f));
        glUniformMatrix4fv(glGetUniformLocation(program.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        RDash->Draw(program);

        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(program.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //glUniform3f(glGetUniformLocation(program.GetProgram(), "material.ambient"), 1.0f, 0.5f, 0.31f);
        glUniform1i(glGetUniformLocation(program.GetProgram(), "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(program.GetProgram(), "material.specular"), 1);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "material.shininess"), 32.0f);
        glUniform1i(glGetUniformLocation(program.GetProgram(), "gamma"), false);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "viewPos"), camera.position.x, camera.position.y, camera.position.z);

        // Directional light
        //glUniform3f(glGetUniformLocation(program.GetProgram(), "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        //glUniform3f(glGetUniformLocation(program.GetProgram(), "dirLight.ambient"), 0.01f, 0.01f, 0.01f);
        //glUniform3f(glGetUniformLocation(program.GetProgram(), "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
        //glUniform3f(glGetUniformLocation(program.GetProgram(), "dirLight.specular"), 0.3f, 0.3f, 0.3f);
        //light.Draw(camera);
        // Point light 1
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[0].quadratic"), 0.032f);
        // Point light 2
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[1].quadratic"), 0.032f);
        // Point light 3
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[2].diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[2].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[2].quadratic"), 0.032f);
        // Point light 4
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[3].diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(program.GetProgram(), "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[3].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(program.GetProgram(), "pointLights[3].quadratic"), 0.032f);

        //glUniformMatrix4fv(glGetUniformLocation(program.GetProgram(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        RenderCubes(VAO, model, 20.0f, glm::vec3(1.0f, 0.3f, 0.5f), program, texture, textureSpec, 10, cubePositions);
        glBindTexture(GL_TEXTURE_2D, 0);

        model = glm::mat4();
        RenderPlane(planeVAO, model,glm::vec3(1.2f, 0.0f, 2.0f), glm::vec3(4.2f), program, planeTex, planeTexSpec);
        glBindTexture(GL_TEXTURE_2D, 0);

        lightProg->load();
        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(lightProg->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightProg->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(model, glm::vec3(1.2f, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightProg->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAOl);
        for (GLuint i=0; i<4; i++)
        {
            model = glm::mat4();
            //model = glm::rotate(model, 10.0f * glm::sin((GLfloat)glfwGetTime()), glm::vec3(0.0f, 0.3f,0.0f));
            model = glm::translate(model, pointLightPositions[i] * glm::sin((GLfloat)glfwGetTime()));
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(glGetUniformLocation(lightProg->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //// Render framebuffer into a quad
        glClearColor(1.0,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        screenProg.load();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);

}

GLuint GetTextureFromFile(std::string filepath, bool sRGB)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char* image = SOIL_load_image(filepath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, sRGB ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}

void RenderPlane(GLuint VAO, glm::mat4 model, glm::vec3 position, glm::vec3 scale, GLSLShader mainshader, GLuint diffTex, GLuint specTex)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specTex);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(glGetUniformLocation(mainshader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray(0);
}

void RenderCubes(GLuint VAO, glm::mat4 model, float angle, glm::vec3 rotation, GLSLShader mainShader, GLuint diffTex, GLuint specTex, GLuint amount, glm::vec3 cubePositions[])
{
    glBindVertexArray(VAO);
    for (GLuint i=0; i<amount; i++)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specTex);
        angle *= i;
        model = glm::rotate(model, angle, rotation);
        model = glm::translate(model, cubePositions[i]);
        glUniformMatrix4fv(glGetUniformLocation(mainShader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
    }
    glBindVertexArray(0);
}

GLuint generateAttachmentTexture(bool depth, bool stencil)
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
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)attachment_type, winWidth, winHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, winWidth, winHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
