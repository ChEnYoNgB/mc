#include "camera.h"
#include "terrain.h"
#include "shadermanager.h"

class SkyBox
{
public:
    SkyBox();
    void Draw(ShaderManager& shaders, Camera& camera);
private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    std::size_t vertices_nums = 0;
    std::size_t indices_nums = 0;
    std::vector<CubeVertex> vertices{
        // 前面，纹理层 1
        {0, 0, 1, 0, 0, 1}, {1, 0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1}, {0, 1, 1, 0, 1, 1},

        // 后面，纹理层 1
        {1, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 1},
        {0, 1, 0, 1, 1, 1}, {1, 1, 0, 0, 1, 1},

        // 左面，纹理层 1
        {0, 0, 0, 0, 0, 1}, {0, 0, 1, 1, 0, 1},
        {0, 1, 1, 1, 1, 1}, {0, 1, 0, 0, 1, 1},

        // 右面，纹理层 1
        {1, 0, 1, 0, 0, 1}, {1, 0, 0, 1, 0, 1},
        {1, 1, 0, 1, 1, 1}, {1, 1, 1, 0, 1, 1},

        // 底面，纹理层 0
        {0, 0, 0, 0, 0, 0}, {1, 0, 0, 1, 0, 0},
        {1, 0, 1, 1, 1, 0}, {0, 0, 1, 0, 1, 0},

        // 顶面，纹理层 2
        {0, 1, 1, 0, 0, 2}, {1, 1, 1, 1, 0, 2},
        {1, 1, 0, 1, 1, 2}, {0, 1, 0, 0, 1, 2},
    };

    std::vector<std::uint32_t> indices{
        0,  1,  2,   2,  3,  0,
        4,  5,  6,   6,  7,  4,
        8,  9, 10,  10, 11,  8,
        12, 13, 14,  14, 15, 12,
        16, 17, 18,  18, 19, 16,
        20, 21, 22,  22, 23, 20,
    };
};