#pragma once
class Map
{
public:
    static constexpr int Size = 16;
    static constexpr int Border = 1;
    static constexpr int StorageSize = Size + Border * 2;
    Map(unsigned int seed);
    void Generate();
    void Generate(int chunkX, int chunkZ);
    int GetHeight(int x, int z) const;
    int GetHeightWithBorder(int x, int z) const;
    inline int GetSize()const{return Size;};
private:
    static constexpr float frequency = 0.05f;
    static constexpr float amplitude = 8.0f;
    static constexpr int baseHeight = 10;
    unsigned int seed;
    int heightMap[StorageSize][StorageSize]{};
};