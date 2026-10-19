#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float speed)
:cameraPos(position),cameraFront(front),cameraUp(up),cameraSpeed(speed)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

Camera::Camera(
    const glm::vec3& position, 
    const glm::vec3& front, 
    const glm::vec3& up, 
    float speed,
    float fov,
    float aspect,
    float near,
    float far)
:cameraPos(position),cameraFront(front),cameraUp(up),cameraSpeed(speed),fov(fov),aspect(aspect),near(near),far(far)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(fov, aspect, near, far);
}


void Camera::MoveForward(float deltatime)
{
    cameraPos += cameraSpeed * deltatime * cameraFront;
}

void Camera::MoveBack(float deltatime)
{
    cameraPos -= cameraSpeed * deltatime * cameraFront;
}

void Camera::MoveUp(float deltatime)
{
    cameraPos += cameraSpeed * deltatime * cameraUp;
}

void Camera::MoveDown(float deltatime)
{
    cameraPos -= cameraSpeed * deltatime * cameraUp;
}

void Camera::MoveLeft(float deltatime)
{
    cameraPos += cameraSpeed * deltatime * glm::normalize(glm::cross(cameraUp, cameraFront));
}

void Camera::MoveRight(float deltatime)
{
    cameraPos -= cameraSpeed * deltatime * glm::normalize(glm::cross(cameraUp, cameraFront));
}

void Camera::MoveMouse(float offsetx, float offsety)
{
    yaw += offsetx * sensitivity;
    pitch += offsety * sensitivity;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

const glm::mat4& Camera::GetView()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}

const glm::mat4& Camera::GetProjection()
{
    projection = glm::perspective(fov, aspect, near, far);
    return projection;
}
