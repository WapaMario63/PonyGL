#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera_Info
{
    enum Cam_Movement
    {
        FORWARDS,
        BACKWARDS,
        LEFT,
        RIGHT,
        UPWARDS,
        DOWNWARDS,
        SLOW,
        FAST,
        NONE,
    };
    constexpr static GLfloat YAW           = -90.0f;
    constexpr static GLfloat PITCH         =  0.0f;
    constexpr static GLfloat SPEED         =  3.0f;
    constexpr static GLfloat SENSITIVITY   =  0.25f;
    constexpr static GLfloat ZOOM          =  45.0f;
};

class Camera
{   
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    Camera(glm::vec3 Position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 Up = glm::vec3(0.0f,1.0f,0.0f), GLfloat Yaw = Camera_Info::YAW, GLfloat Pitch = Camera_Info::PITCH);
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat Yaw, GLfloat Pitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Info::Cam_Movement direction, GLfloat deltaTime);
    void ProcessMouseMoveMent(GLfloat xOffset, GLfloat yOffset, bool constrainPitch = true);
    void ProcessMouseScroll(GLfloat yOffset);
private:
    void UpdateCameraVectors();
};

#endif // CAMERA_H
