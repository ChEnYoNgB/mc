#include <cassert>
#include "map.h"
#include "perlinnoise.h"

Map::Map(unsigned int seed):seed(seed)
{

};

void Map::Generate()
{
    Generate(0, 0);
};

void Map::Generate(int chunkX, int chunkZ)
{
    PerlinNoise noise{seed};
    for (int x = -CHUNK_BORDER; x < CHUNK_SIZE + CHUNK_BORDER; ++x)
    {
        for (int z = -CHUNK_BORDER; z < CHUNK_SIZE + CHUNK_BORDER; ++z)
        {
            int worldX = chunkX * CHUNK_SIZE + x;
            int worldZ = chunkZ * CHUNK_SIZE + z;
            float n = noise.Noise(worldX * frequency, worldZ * frequency);
            int height = baseHeight + static_cast<int>(n * amplitude);
            heightMap[Index(x + CHUNK_BORDER, z + CHUNK_BORDER)] = std::max(1, height);
        }
    }
}

int Map::GetHeight(int x, int z)const
{
    assert(x >= 0 && x < Size);
    assert(z >= 0 && z < Size);
    return heightMap[Index(x + CHUNK_BORDER, z + CHUNK_BORDER)];
}

int Map::GetHeightWithBorder(int x, int z)const
{
    assert(x >= -Border && x < Size + Border);
    assert(z >= -Border && z < Size + Border);
    return heightMap[Index(x + CHUNK_BORDER, z + CHUNK_BORDER)];
}

std::size_t Map::Index(int index_x, int index_y)const
{
    return static_cast<std::size_t>(index_y) * StorageSize + index_x;
}