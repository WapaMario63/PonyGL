#include "Camera.h"

Camera::Camera(glm::vec3 Position, glm::vec3 Up, GLfloat Yaw, GLfloat Pitch)
    : front(glm::vec3(0.0f,0.0f,-1.0f)),
      movementSpeed(Camera_Info::SPEED),
      mouseSensitivity(Camera_Info::SENSITIVITY),
      zoom(Camera_Info::ZOOM)
{
    this->position = Position;
    this->worldUp = Up;
    this->yaw = Yaw;
    this->pitch = Pitch;
    this->UpdateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat Yaw, GLfloat Pitch)
    : front(glm::vec3(0.0f,0.0f,-1.0f)),
      movementSpeed(Camera_Info::SPEED),
      mouseSensitivity(Camera_Info::SENSITIVITY),
      zoom(Camera_Info::ZOOM)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = Yaw;
    this->pitch = Pitch;
    this->UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt
    (
        this->position,
        this->position + this->front,
        this->up
    );
}

void Camera::ProcessKeyboard(Camera_Info::Cam_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->movementSpeed * deltaTime;
    if (direction == Camera_Info::Cam_Movement::FORWARDS)
        this->position += this->front * velocity;
    if (direction == Camera_Info::Cam_Movement::BACKWARDS)
        this->position -= this->front * velocity;
    if (direction == Camera_Info::Cam_Movement::LEFT)
        this->position -= this->right * velocity;
    if (direction == Camera_Info::Cam_Movement::RIGHT)
        this->position += this->right * velocity;
    if (direction == Camera_Info::Cam_Movement::UPWARDS)
        this->position += this->up * velocity;
    if (direction == Camera_Info::Cam_Movement::DOWNWARDS)
        this->position -= this->up * velocity;

    this->UpdateCameraVectors();
}

void Camera::ProcessMouseMoveMent(GLfloat xOffset, GLfloat yOffset, bool constrainPitch)
{
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw   += xOffset;
    this->pitch += yOffset;

    if (constrainPitch)
    {
        if (this->pitch >  89.0f) this->pitch =  89.0f;
        if (this->pitch < -89.0f) this->pitch = -89.0f;
    }

    this->UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yOffset)
{
    if (this->zoom >= 1.0f && this->zoom <= 45.0f) this->zoom -= yOffset;
    if (this->zoom <= 1.0f) this->zoom = 1.0f;
    if (this->zoom >= 45.0f) this->zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    if (this->yaw > 360.0f)
        this->yaw = 0.0f;
    else if (this->yaw < 0.0f)
        this->yaw = 360.0f;

    glm::vec3 mFront;
    mFront.x = (float)cos(glm::radians(this->yaw)) * (float)cos(glm::radians(this->pitch));
    mFront.y = static_cast<float>(sin(glm::radians(this->pitch)));
    mFront.z = (float)sin(glm::radians(this->yaw)) * (float)cos(glm::radians(this->pitch));

    this->front = glm::normalize(mFront);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up    = glm::normalize(glm::cross(this->right, this->front));

}
