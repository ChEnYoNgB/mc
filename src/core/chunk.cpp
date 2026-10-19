#include "chunk.h"

Chunk::Chunk(int x, int z, unsigned int seed)
    : x(x), z(z)
{
    map = std::make_unique<Map>(seed);
    terrain = std::make_unique<Terrain>();

    map->Generate(x, z);
    terrain->BuildTerrainMesh(*map);

    model = glm::translate(
        glm::mat4{1.0f},
        glm::vec3{static_cast<float>(x * map->GetSize()), 0.0f, static_cast<float>(z * map->GetSize())}
    );
}

void Chunk::Draw()const
{
    if(terrain)
    {
        terrain->Draw();
    }
}