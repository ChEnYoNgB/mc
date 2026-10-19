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
    for (int x = -Border; x < Size + Border; ++x)
    {
        for (int z = -Border; z < Size + Border; ++z)
        {
            int worldX = chunkX * Size + x;
            int worldZ = chunkZ * Size + z;
            float n = noise.Noise(worldX * frequency, worldZ * frequency);
            int height = baseHeight + static_cast<int>(n * amplitude);
            heightMap[x + Border][z + Border] = std::max(1, height);
        }
    }
}

int Map::GetHeight(int x, int z)const
{
    assert(x >= 0 && x < Size);
    assert(z >= 0 && z < Size);

    return heightMap[x + Border][z + Border];
}

int Map::GetHeightWithBorder(int x, int z)const
{
    assert(x >= -Border && x < Size + Border);
    assert(z >= -Border && z < Size + Border);

    return heightMap[x + Border][z + Border];
}