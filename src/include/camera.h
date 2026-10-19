#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float speed);
    Camera(
        const glm::vec3& position, 
        const glm::vec3& front, 
        const glm::vec3& up, 
        float speed,
        float fov,
        float aspect,
        float near,
        float far
    );
    void MoveForward(float deltatime);
    void MoveBack(float deltatime);
    void MoveUp(float deltatime);
    void MoveDown(float deltatime);
    void MoveLeft(float deltatime);
    void MoveRight(float deltatime);
    void MoveMouse(float offsetx, float offsety);
    inline float GetFov()const{return fov;};
    inline float GetAspect()const{return aspect;};
    inline float GetNear()const{return near;};
    inline float GetFar()const{return far;};
    const glm::mat4& GetView();
    const glm::mat4& GetProjection();
    const glm::vec3& GetWoldPosition()const {return cameraPos;};
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
private:
    float cameraSpeed = 0.05f;
    float sensitivity = 0.3f;
    float yaw = -90.f;
    float pitch = 0;
    float fov = glm::radians(45.0f);
    float aspect = 640.0f / 480.0f;
    float near = 0.1f;
    float far = 100.0f;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    
    glm::mat4 view;
    glm::mat4 projection;
};