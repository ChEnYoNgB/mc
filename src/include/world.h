#include "chunk.h"
#include "camera.h"
#include "shadermanager.h"

struct ChunkCoord
{
    int x;
    int z;

    bool operator==(const ChunkCoord& other) const
    {
        return x == other.x && z == other.z;
    }
};

struct ChunkCoordHash
{
    std::size_t operator()(const ChunkCoord& coord) const
    {
        std::size_t h1 = std::hash<int>{}(coord.x);
        std::size_t h2 = std::hash<int>{}(coord.z);
        return h1 ^ (h2 << 1);
    }
};

class World
{
public:
    World(unsigned int seed):seed_(seed){};
    void Update(const Camera& camera);
    void Draw(ShaderManager& shader_manager, Camera& camera);

private:
    unsigned int seed_;
    int radius = 16;
    constexpr static int CHUNK_SIZE = 16;
    int currentChunkX_ = 999999;
    int currentChunkZ_ = 999999;
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks_;

    int WorldToChunk(float value);
    void LoadChunk(const ChunkCoord& coord);
    void UnloadFarChunks(int chunkX, int chunkZ);
};