#pragma once
#include <vector>
#include "config.h"
#include "block_type.h"
class Map
{
public:
    int StorageSize = CHUNK_SIZE + CHUNK_BORDER * 2;
    Map(unsigned int seed);
    void Generate();
    void Generate(int chunkX, int chunkZ);
    int GetHeight(int x, int z) const;
    int GetHeightWithBorder(int x, int z) const;
    inline int GetSize()const{return CHUNK_SIZE;};
private:
    static constexpr float frequency = 0.05f;
    static constexpr float amplitude = 8.0f;
    static constexpr int baseHeight = 10;
    unsigned int seed;
    std::vector<int> heightMap = std::vector<int>(StorageSize * StorageSize, 0);
    std::vector<BlockType> blockCache{static_cast<std::size_t>(CHUNK_SIZE) * CHUNK_HEIGHT * CHUNK_SIZE, BlockType::Air};
    std::size_t Index(int index_x, int index_y)const;
};