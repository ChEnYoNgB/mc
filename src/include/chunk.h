#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "map.h"
#include "terrain.h"

class Chunk
{
public:
    Chunk(int x, int z, unsigned int seed);
    void Draw()const;
    const glm::mat4& GetModel()const {return model;};
private:
    int x;
    int z;
    std::unique_ptr<Map> map;
    std::unique_ptr<Terrain> terrain;
    glm::mat4 model{1.0f};
};