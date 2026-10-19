#pragma once
#include <glm/fwd.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Shader
{
public:
    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    void Use()const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    void SetTexture(const std::string& image_path);
    unsigned int GetShaderProgram()const{return shaderProgram;};
private:
    unsigned int shaderProgram = 0;
    unsigned int texture = 0;
};