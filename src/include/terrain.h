#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include "map.h"
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

struct CubeVertex
{
    float x;
    float y;
    float z;
    float uv0;
    float uv1;
    float tex_index;
    float brightness;
};

class Terrain
{
public:
    Terrain();
    Terrain(const std::vector<CubeVertex>& vertices);
    Terrain(const std::vector<CubeVertex>& vertices, const std::vector<uint32_t>& indices);
    ~Terrain();
    virtual void Draw();
    virtual void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
    void SetShader(unsigned int shader);
    void SetTexture(const std::string image_path);
    void SetUniformMatrix4fv(const std::string& uniform_name, const glm::mat4& data);
    void SetUniformInt(const std::string& uniform_name, int data);
    void BuildTerrainMesh(const Map& map);
    Terrain(const Terrain&) = delete;
    Terrain& operator=(const Terrain&) = delete;

protected:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int texture = 0;
    unsigned int shader = 0;
    std::size_t vertices_nums = 0;
    std::size_t indices_nums = 0;

    std::vector<CubeVertex> vertices{};
    std::vector<std::uint32_t> indices{};

    bool IsSolid(const Map& map, int x, int y, int z);
    void AddFace(const CubeVertex (&face)[4]);
    void AddTopFace(const Map& map, float x, float y, float z);
    void AddFrontFace(const Map& map, float x, float y, float z);
    void AddBehindFace(const Map& map, float x, float y, float z);
    void AddLeftFace(const Map& map, float x, float y, float z);
    void AddRightFace(const Map& map, float x, float y, float z);
    void AddDownFace(const Map& map, float x, float y, float z);
    int VertexAO(bool side1, bool side2, bool corner);
    float AOToBrightness(int ao);
    float CalculateVertexBrightness(
        const Map& map,
        int x,
        int y,
        int z,
        const glm::ivec3& normal,
        const glm::ivec3& sideA,
        const glm::ivec3& sideB,
        float faceBrightness
    );
};