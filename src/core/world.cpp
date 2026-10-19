#include "world.h"
#include "shader.h"

void World::Update(const Camera& camera)
{
    glm::vec3 cameraPosition = camera.GetWoldPosition();
    int chunkX = WorldToChunk(cameraPosition.x);
    int chunkZ =  WorldToChunk(cameraPosition.z);

    if(currentChunkX_ == chunkX && currentChunkZ_ == chunkZ)
        return;

    currentChunkX_ = chunkX;
    currentChunkZ_ = chunkZ;

    for (int dz = -radius; dz <= radius; ++dz)
    {
        for (int dx = -radius; dx <= radius; ++dx)
        {
            ChunkCoord coord{chunkX + dx, chunkZ + dz};
            if (chunks_.find(coord) == chunks_.end())
            {
                LoadChunk(coord);
            }
        }
    }

    UnloadFarChunks(chunkX, chunkZ);
}

void World::Draw(ShaderManager& shader_manager, Camera& camera)
{
    for (auto& [coord, chunk] : chunks_)
    {
        auto& shader = shader_manager.Get("terrain");
        shader.Use();
        shader.SetMat4("model", chunk->GetModel());
        shader.SetMat4("view", camera.GetView());
        shader.SetMat4("projection", camera.GetProjection());
        shader.SetVec3("cameraPosition", camera.GetWoldPosition());
        shader.SetVec3("fogColor", glm::vec3{0.65f, 0.80f,1.00f});
        shader.SetFloat("fogStart", 80.f);
        shader.SetFloat("fogEnd", 100.f);
        chunk->Draw();
    }
}

int World::WorldToChunk(float value)
{
    // return static_cast<int>(std::floor((value + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));
    return static_cast<int>(std::floor(value / Map::Size));
}

void World::LoadChunk(const ChunkCoord& coord)
{
    std::unique_ptr<Chunk> chunk{std::make_unique<Chunk>(coord.x, coord.z, seed_)};
    chunks_.emplace(coord, std::move(chunk));
}

void World::UnloadFarChunks(int chunkX, int chunkZ)
{
    for(auto it = chunks_.begin();it != chunks_.end();)
    {
        const ChunkCoord& coord = it->first;
        int coordX = coord.x;
        int coordZ = coord.z;
        if(std::abs(coordX - chunkX) >= radius || std::abs(coordZ - chunkZ) >= radius)
        {
            it = chunks_.erase(it);
        }
        else{
            ++it;
        }
    }
}