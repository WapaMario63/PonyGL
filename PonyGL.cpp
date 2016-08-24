#include "PonyGL.h"
#include "Render/Shader.h"
#include "Render/Model.h"
#include "Camera.h"
#include "Render/Light.h"
#include "Utils/PneUtilities.h"
#include "Sound/PneSound.h"

#include <fstream>
#include <vector>
#include <string>
#include <math.h>

glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
bool keys[1024];
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = 400;
GLfloat lastY = 300;
GLfloat yaw = 0.0f;
GLfloat pitch = 0.0f;
bool firstMouse = true;
GLfloat fov = 45.0f;

GLfloat charX = 1.0f;
GLfloat charZ = 3.0f;
GLfloat charY = -19.87f;
GLfloat charRX = 1.0f;
GLfloat charRY = 1.0f;
GLfloat charRZ = 1.0f;
bool isjumping = false;

PonyGL::PonyGL()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 16); // 16x antialiasing MSAA
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Ditch the old OpenGL system (fixed-pipeline)
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}
PonyGL::~PonyGL()
{

}

// --------------------------------------------------------------------------------------
//      GLFW CallBack Functions
// --------------------------------------------------------------------------------------

#if defined(__MSVCVER)
#pragma region "User Input"
#endif

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
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xOffset = xpos - lastX;
    GLfloat yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

   camera.ProcessMouseMoveMent(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void joystick_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED)
    {
        printf("Controller detected: %s\n",glfwGetJoystickName(joy));
    }
    else if (event == GLFW_DISCONNECTED)
    {
        printf("Controller Disconnected.\n");
    }
}

void do_movement()
{
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(Camera_Info::FORWARDS, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(Camera_Info::BACKWARDS, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(Camera_Info::LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(Camera_Info::RIGHT, deltaTime);
    if (keys[GLFW_KEY_SPACE]) camera.ProcessKeyboard(Camera_Info::UPWARDS, deltaTime);
    if (keys[GLFW_KEY_LEFT_SHIFT]) camera.ProcessKeyboard(Camera_Info::DOWNWARDS, deltaTime);

    /*if (keys[GLFW_KEY_LEFT]) lightPos -= lightPos.x + 1.0f * deltaTime;
    if (keys[GLFW_KEY_RIGHT]) lightPos += lightPos.x + 1.0f * deltaTime;
    if (keys[GLFW_KEY_UP]) lightPos += lightPos.z + 1.0f * deltaTime;
    if (keys[GLFW_KEY_DOWN]) lightPos -= lightPos.z + 1.0f * deltaTime;*/

    if (keys[GLFW_KEY_R] && GLFW_PRESS) camera.movementSpeed = 0.5f;
    else if (keys[GLFW_KEY_LEFT_CONTROL] && GLFW_PRESS) camera.movementSpeed = 14.0f;
    else camera.movementSpeed = Camera_Info::SPEED;

    // X = left, Z = front
    if (keys[GLFW_KEY_LEFT] && GLFW_PRESS) charX += 0.5f;
    if (keys[GLFW_KEY_RIGHT] && GLFW_PRESS) charX -= 0.5f;
    if (keys[GLFW_KEY_UP] && GLFW_PRESS) charZ += 0.5f;
    if (keys[GLFW_KEY_DOWN] && GLFW_PRESS) charZ -= 0.5f;

    if (keys[GLFW_KEY_DELETE] && GLFW_PRESS) charRX += 0.5f;
    if (keys[GLFW_KEY_PAGE_DOWN] && GLFW_PRESS) charRX -= 0.5f;
    if (keys[GLFW_KEY_INSERT] && GLFW_PRESS) charRZ += 0.5f;
    if (keys[GLFW_KEY_PAGE_UP] && GLFW_PRESS) charRZ -= 0.5f;
    if (keys[GLFW_KEY_HOME] && GLFW_PRESS) charRY += 0.5f;
    if (keys[GLFW_KEY_END] && GLFW_PRESS) charRY -= 0.5f;


}

#if defined(__MSVCVER)
#pragma endregion
#endif

// --------------------------------------------------------------------------------------
//      GET Functions
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
//      Helper Functions
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
//      Main Functions (Initializer and Drawer)
// --------------------------------------------------------------------------------------
void PonyGL::initializeGL()
{
    PneUtilities utils;

    window = glfwCreateWindow(winWidth, winHeight, "PonyGL | Initializing...", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    if (utils.IsBigEndian())
    {
        std::cout << "CPU is Big Endian" << std::endl;
    }
    else
    {
        std::cout << "CPU is Little Endian" << std::endl;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetCursorPos(window, (double)winWidth/2,(double)winHeight/2);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetJoystickCallback(joystick_callback);
    if (glfwJoystickPresent(GLFW_JOYSTICK_1))
    {
        printf("Controller detected: %s\n",glfwGetJoystickName(GLFW_JOYSTICK_1));
    }


    glViewport(0,0, winWidth, winHeight);


    GLfloat vertices[] =
    {
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Light Stuff
    GLuint VBO, lVAO;
    glGenVertexArrays(1, &lVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // ----

    //Shader lightShader = Shader("res/shader/LightVertexShader.pvs", "res/shader/LightFragmentShader.pvs");
    //Shader lightObjShader = Shader("res/shader/LightVertexShader.pvs", "res/shader/LightCubeFragmentShader.pfs");
    glfwSetWindowTitle(window, "PonyGL | Loading Sound...");
    //PneSound* sound = new PneSound("duel_challenge.wav", PneSound::SndType::SndFX2D);

    glfwSetWindowTitle(window, "PonyGL | Loading Models...");
    Shader modelShader = Shader("res/shader/ModelVertexShader.pvs", "res/shader/ModelFragmentShader.pfs");
    Shader directLight = Shader("res/shader/Light_DirectionalV.pvs", "res/shader/Light_DirectionalF.pfs");
    Model* rdModel = new Model("res/model/rdModelMat.obj");
    Model* OgreM = new Model("res/model/Ice_fortress_act1.obj");

    Light light = Light(camera);
    light.setLight(Light::DIRECTIONAL);
    light.setColor(glm::vec3(1.0f,1.0f,1.0f));
    light.setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
    light.setDirectionalLightShader(directLight);

    glfwSetWindowTitle(window, "PonyGL | Setting Up Scene...");
    //sound->Play();
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        do_movement();

        // Render
        // Clear the color buffer
        glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //modelShader.use();
        light.getDirectionalLightShader().use();

        // Light Stuff
        //lightShader.use();
        GLint objectColorLoc = glGetUniformLocation(modelShader.Program, "objectColor");
        GLint lightColorLoc = glGetUniformLocation(modelShader.Program, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(modelShader.Program, "lightPos");
        GLint viewPosLoc = glGetUniformLocation(modelShader.Program, "viewPos");
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightColorLoc, 1.0f, 0.980f, 1.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);

        // Create transformations
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.zoom, (GLfloat)winWidth / (GLfloat)winHeight, 0.1f, 100.0f);

        // Draw a Model
        // Get their uniform location
        glUniformMatrix4fv(glGetUniformLocation(light.getDirectionalLightShader().Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(light.getDirectionalLightShader().Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(model, glm::vec3(charX, charY, charZ));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        model = glm::rotate(model,360.0f, glm::vec3(charRX, charRY, charRZ));
        glUniformMatrix4fv(glGetUniformLocation(light.getDirectionalLightShader().Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        rdModel->Draw(light.getDirectionalLightShader());

        // Draw another model
        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(light.getDirectionalLightShader().Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(light.getDirectionalLightShader().Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(model, glm::vec3(-50.0f, -20.7, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(light.getDirectionalLightShader().Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        OgreM->Draw(light.getDirectionalLightShader());

        // Draw a Light Object
        //lightObjShader.use();

        /*GLint modelLoc = glGetUniformLocation(light.getDirectionalLightShader().Program, "model");
        GLint viewLoc = glGetUniformLocation(light.getDirectionalLightShader().Program, "view");
        GLint projLoc = glGetUniformLocation(light.getDirectionalLightShader().Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightPos.y = 1.0f / sin(glfwGetTime()) * 2.0f;
        lightPos.z = cos(glfwGetTime() * 2.0f) * 7.0f;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



        light.Draw();*/
        glBindVertexArray(lVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glfwSetWindowTitle(window, "PonyGL");

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    //sound->Stop();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void PonyGL::drawGL()
{

}

