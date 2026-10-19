#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

struct Vertex
{
    float x;
    float y;
    float z;
    float uv0;
    float uv1;
};

class Geometry
{
public:
    Geometry(const std::vector<Vertex>& vertices);
    Geometry(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
    ~Geometry();
    virtual void Draw();
    void SetShader(unsigned int shader);
    void SetTexture(const std::string image_path);
    void SetUniformMatrix4fv(const std::string& uniform_name, const glm::mat4& data);
    void SetUniformInt(const std::string& uniform_name, int data);
    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;

protected:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int texture = 0;
    unsigned int shader = 0;
    std::size_t vertices_nums = 0;
    std::size_t indices_nums = 0;
};