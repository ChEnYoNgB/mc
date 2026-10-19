#include "stb_image.h"
#include "terrain.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Terrain::Terrain()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
};

Terrain::Terrain(const std::vector<CubeVertex>& vertices)
{
    vertices_nums = vertices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_nums * sizeof(CubeVertex)), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
};

Terrain::Terrain(const std::vector<CubeVertex>& vertices, const std::vector<uint32_t>& indices)
{
    vertices_nums = vertices.size();
    indices_nums = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_nums * sizeof(CubeVertex)), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices_nums * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
};

void Terrain::Draw()
{
    // glUseProgram(shader);
    // if(texture != 0)
    // {
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, texture);
    // }

    glBindVertexArray(VAO);
    if(EBO != 0)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_nums), GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_nums));
    }
    glBindVertexArray(0);
}

void Terrain::Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj)
{
    // glUseProgram(shader);

    // SetUniformMatrix4fv("view", view);
    // SetUniformMatrix4fv("projection", proj);
    // SetUniformMatrix4fv("model", model);

    // if(texture != 0)
    // {
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, texture);
    // }

    glBindVertexArray(VAO);
    if(EBO != 0)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_nums), GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_nums));
    }
    glBindVertexArray(0);
}

void Terrain::SetUniformMatrix4fv(const std::string& uniform_name, const glm::mat4& data)
{
    int modelLoc = glGetUniformLocation(shader, uniform_name.c_str());
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(data));
}

void Terrain::SetUniformInt(const std::string& uniform_name, int data)
{
    int modelLoc = glGetUniformLocation(shader, uniform_name.c_str());
    glUniform1i(modelLoc, data);
}

void Terrain::SetShader(unsigned int shader)
{
    this->shader = shader;
}

void Terrain::SetTexture(const std::string image_path)
{
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
        {
            std::cout << "Unsupported texture channel count: "
                    << nrChannels << std::endl;
            stbi_image_free(data);
            return;
        }

        if (format == GL_RGB)
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Terrain::BuildTerrainMesh(const Map& map)
{
    vertices.clear();
    indices.clear();
    int size = map.GetSize();
    for(int x = 0; x < size; ++x)
    {
        for(int z = 0; z < size; ++z)
        {
            int height = map.GetHeight(x, z);
            for(int y=0; y < height; ++y)
            {
                if(!IsSolid(map, x + 1, y, z))
                {
                    //TODO add right face
                    AddRightFace(map, x, y, z);
                }

                if(!IsSolid(map, x - 1, y, z))
                {
                    //TODO add left face
                    AddLeftFace(map, x, y, z);
                }

                if(!IsSolid(map, x, y + 1, z))
                {
                    //TODO add top face
                    AddTopFace(map, x, y, z);
                }

                if(!IsSolid(map, x, y - 1, z))
                {
                    //TODO add down face
                    AddDownFace(map, x, y, z);
                }

                if(!IsSolid(map, x, y, z - 1))
                {
                    //TODO add behind face
                    AddBehindFace(map, x, y, z);
                }

                if(!IsSolid(map, x, y, z + 1))
                {
                    //TODO add front face
                    AddFrontFace(map, x, y, z);
                }
            }
        }
    }

    vertices_nums = vertices.size();
    indices_nums = indices.size();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_nums * sizeof(CubeVertex)), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices_nums * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Terrain::~Terrain()
{
    if(EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
    }

    if (texture != 0)
    {
        glDeleteTextures(1, &texture);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
};

bool Terrain::IsSolid(const Map& map, int x, int y, int z)
{
    // 保持原先的底部处理：
    // y = 0 的方块会生成朝下的底面。
    if (y < 0)
        return false;

    // BuildTerrainMesh 只查询紧邻方块，
    // 因此 x、z 最多越界一格，边界缓存足够覆盖。
    return y < map.GetHeightWithBorder(x, z);
}

void Terrain::AddFace(const CubeVertex (&face)[4])
{
    unsigned int base = static_cast<unsigned int>(vertices.size());

    for (const auto& vertex : face)
    {
        vertices.push_back(vertex);
    }

    indices.push_back(base + 0);
    indices.push_back(base + 1);
    indices.push_back(base + 2);

    indices.push_back(base + 2);
    indices.push_back(base + 3);
    indices.push_back(base + 0);
}

void Terrain::AddTopFace(const Map& map, float x, float y, float z)
{
    // constexpr float textureIndex = 2.0f;
    // CubeVertex face[4] =
    // {
    //     {x, y + 1, z + 1, 0.0f, 0.0f, textureIndex, 1.0},
    //     {x + 1, y + 1, z + 1, 1.0f, 0.0f, textureIndex, 1.0},
    //     {x + 1, y + 1, z, 1.0f, 1.0f, textureIndex, 1.0},
    //     {x, y + 1, z, 0.0f, 1.0f, textureIndex, 1.0}
    // };
    // AddFace(face);
    constexpr float textureIndex = 2.0f;
    constexpr float faceBrightness = 1.0f;

    float b0 = CalculateVertexBrightness(
        map,
        x, y, z,
        {0, 1, 0},
        {-1, 0, 0},
        {0, 0, 1},
        faceBrightness
    );

    float b1 = CalculateVertexBrightness(
        map,
        x, y, z,
        {0, 1, 0},
        {1, 0, 0},
        {0, 0, 1},
        faceBrightness
    );

    float b2 = CalculateVertexBrightness(
        map,
        x, y, z,
        {0, 1, 0},
        {1, 0, 0},
        {0, 0, -1},
        faceBrightness
    );

    float b3 = CalculateVertexBrightness(
        map,
        x, y, z,
        {0, 1, 0},
        {-1, 0, 0},
        {0, 0, -1},
        faceBrightness
    );

    CubeVertex face[4] =
    {
        {x,     y + 1, z + 1, 0.0f, 0.0f, textureIndex, b0},
        {x + 1, y + 1, z + 1, 1.0f, 0.0f, textureIndex, b1},
        {x + 1, y + 1, z,     1.0f, 1.0f, textureIndex, b2},
        {x,     y + 1, z,     0.0f, 1.0f, textureIndex, b3}
    };

    AddFace(face);
}

void Terrain::AddFrontFace(const Map& map, float x, float y, float z)
{
    // constexpr float textureIndex = 1.0f;
    // CubeVertex face[4] =
    // {
    //     {x, y, z + 1, 0.0f, 0.0f, textureIndex, 0.65},
    //     {x + 1, y, z + 1, 1.0f, 0.0f, textureIndex, 0.65},
    //     {x + 1, y + 1, z + 1, 1.0f, 1.0f, textureIndex, 0.65},
    //     {x, y + 1, z + 1, 0.0f, 1.0f, textureIndex, 0.65}
    // };
    // AddFace(face);
    constexpr float faceBrightness = 0.8f;

    float b0 = CalculateVertexBrightness(
        map, x, y, z,
        {0,0,1},
        {-1,0,0},
        {0,-1,0},
        faceBrightness
    );

    float b1 = CalculateVertexBrightness(
        map, x, y, z,
        {0,0,1},
        {1,0,0},
        {0,-1,0},
        faceBrightness
    );

    float b2 = CalculateVertexBrightness(
        map, x, y, z,
        {0,0,1},
        {1,0,0},
        {0,1,0},
        faceBrightness
    );

    float b3 = CalculateVertexBrightness(
        map, x, y, z,
        {0,0,1},
        {-1,0,0},
        {0,1,0},
        faceBrightness
    );

    CubeVertex face[] =
    {
        {
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(z + 1),
            0.0f,
            0.0f,
            1.0f,   // texture index
            b0
        },

        {
            static_cast<float>(x + 1),
            static_cast<float>(y),
            static_cast<float>(z + 1),
            1.0f,
            0.0f,
            1.0f,
            b1
        },

        {
            static_cast<float>(x + 1),
            static_cast<float>(y + 1),
            static_cast<float>(z + 1),
            1.0f,
            1.0f,
            1.0f,
            b2
        },

        {
            static_cast<float>(x),
            static_cast<float>(y + 1),
            static_cast<float>(z + 1),
            0.0f,
            1.0f,
            1.0f,
            b3
        }
    };

    AddFace(face);
}

void Terrain::AddBehindFace(const Map& map, float x, float y, float z)
{
    // constexpr float textureIndex = 1.0f;
    // CubeVertex face[] = {
    //     {x + 1, y,     z, 0.0f, 0.0f, textureIndex, 0.65},
    //     {x,     y,     z, 1.0f, 0.0f, textureIndex, 0.65},
    //     {x,     y + 1, z, 1.0f, 1.0f, textureIndex, 0.65},
    //     {x + 1, y + 1, z, 0.0f, 1.0f, textureIndex, 0.65},
    // };
    // AddFace(face);
    constexpr float textureIndex = 1.0f;
    constexpr float faceBrightness = 0.65f;

    // Behind = -Z
    //
    // v0 = (x+1, y,   z)   => +X, -Y
    // v1 = (x,   y,   z)   => -X, -Y
    // v2 = (x,   y+1, z)   => -X, +Y
    // v3 = (x+1, y+1, z)   => +X, +Y

    float b0 = CalculateVertexBrightness(
        map, x, y, z,
        {0, 0, -1},     // normal = -Z
        {1, 0, 0},      // +X
        {0, -1, 0},     // -Y
        faceBrightness
    );

    float b1 = CalculateVertexBrightness(
        map, x, y, z,
        {0, 0, -1},
        {-1, 0, 0},     // -X
        {0, -1, 0},     // -Y
        faceBrightness
    );

    float b2 = CalculateVertexBrightness(
        map, x, y, z,
        {0, 0, -1},
        {-1, 0, 0},     // -X
        {0, 1, 0},      // +Y
        faceBrightness
    );

    float b3 = CalculateVertexBrightness(
        map, x, y, z,
        {0, 0, -1},
        {1, 0, 0},      // +X
        {0, 1, 0},      // +Y
        faceBrightness
    );

    CubeVertex face[] = {
        {x + 1, y,     z, 0.0f, 0.0f, textureIndex, b0},
        {x,     y,     z, 1.0f, 0.0f, textureIndex, b1},
        {x,     y + 1, z, 1.0f, 1.0f, textureIndex, b2},
        {x + 1, y + 1, z, 0.0f, 1.0f, textureIndex, b3},
    };

    AddFace(face);
}

void Terrain::AddLeftFace(const Map& map, float x, float y, float z)
{
    // constexpr float textureIndex = 1.0f;
    // CubeVertex face[] = {
    //     {x, y,     z,     0.0f, 0.0f, textureIndex, 0.8},
    //     {x, y,     z + 1, 1.0f, 0.0f, textureIndex, 0.8},
    //     {x, y + 1, z + 1, 1.0f, 1.0f, textureIndex, 0.8},
    //     {x, y + 1, z,     0.0f, 1.0f, textureIndex, 0.8},
    // };
    // AddFace(face);
    constexpr float textureIndex = 1.0f;
    constexpr float faceBrightness = 0.8f;

    // Left = -X
    //
    // v0 = (x, y,   z)     => -Z, -Y
    // v1 = (x, y,   z+1)   => +Z, -Y
    // v2 = (x, y+1, z+1)   => +Z, +Y
    // v3 = (x, y+1, z)     => -Z, +Y

    float b0 = CalculateVertexBrightness(
        map, x, y, z,
        {-1, 0, 0},     // normal = -X
        {0, 0, -1},     // -Z
        {0, -1, 0},     // -Y
        faceBrightness
    );

    float b1 = CalculateVertexBrightness(
        map, x, y, z,
        {-1, 0, 0},
        {0, 0, 1},      // +Z
        {0, -1, 0},     // -Y
        faceBrightness
    );

    float b2 = CalculateVertexBrightness(
        map, x, y, z,
        {-1, 0, 0},
        {0, 0, 1},      // +Z
        {0, 1, 0},      // +Y
        faceBrightness
    );

    float b3 = CalculateVertexBrightness(
        map, x, y, z,
        {-1, 0, 0},
        {0, 0, -1},     // -Z
        {0, 1, 0},      // +Y
        faceBrightness
    );

    CubeVertex face[] = {
        {x, y,     z,     0.0f, 0.0f, textureIndex, b0},
        {x, y,     z + 1, 1.0f, 0.0f, textureIndex, b1},
        {x, y + 1, z + 1, 1.0f, 1.0f, textureIndex, b2},
        {x, y + 1, z,     0.0f, 1.0f, textureIndex, b3},
    };

    AddFace(face);
}

void Terrain::AddRightFace(const Map& map, float x, float y, float z)
{
    // constexpr float textureIndex = 1.0f;
    // CubeVertex face[] = {
    //     {x + 1, y,     z + 1, 0.0f, 0.0f, textureIndex, 0.8},
    //     {x + 1, y,     z,     1.0f, 0.0f, textureIndex, 0.8},
    //     {x + 1, y + 1, z,     1.0f, 1.0f, textureIndex, 0.8},
    //     {x + 1, y + 1, z + 1, 0.0f, 1.0f, textureIndex, 0.8},
    // };
    // AddFace(face);
    constexpr float textureIndex = 1.0f;
    constexpr float faceBrightness = 0.8f;

    // Right = +X
    //
    // v0 = (x+1, y,   z+1) => +Z, -Y
    // v1 = (x+1, y,   z)   => -Z, -Y
    // v2 = (x+1, y+1, z)   => -Z, +Y
    // v3 = (x+1, y+1, z+1) => +Z, +Y

    float b0 = CalculateVertexBrightness(
        map, x, y, z,
        {1, 0, 0},      // normal = +X
        {0, 0, 1},      // +Z
        {0, -1, 0},     // -Y
        faceBrightness
    );

    float b1 = CalculateVertexBrightness(
        map, x, y, z,
        {1, 0, 0},
        {0, 0, -1},     // -Z
        {0, -1, 0},     // -Y
        faceBrightness
    );

    float b2 = CalculateVertexBrightness(
        map, x, y, z,
        {1, 0, 0},
        {0, 0, -1},     // -Z
        {0, 1, 0},      // +Y
        faceBrightness
    );

    float b3 = CalculateVertexBrightness(
        map, x, y, z,
        {1, 0, 0},
        {0, 0, 1},      // +Z
        {0, 1, 0},      // +Y
        faceBrightness
    );

    CubeVertex face[] = {
        {x + 1, y,     z + 1, 0.0f, 0.0f, textureIndex, b0},
        {x + 1, y,     z,     1.0f, 0.0f, textureIndex, b1},
        {x + 1, y + 1, z,     1.0f, 1.0f, textureIndex, b2},
        {x + 1, y + 1, z + 1, 0.0f, 1.0f, textureIndex, b3},
    };

    AddFace(face);
}

void Terrain::AddDownFace(const Map& map, float x, float y, float z)
{
    // constexpr float textureIndex = 0.0f;
    // CubeVertex face[] = {
    //     {x,     y, z,     0.0f, 0.0f, textureIndex, 0.5},
    //     {x + 1, y, z,     1.0f, 0.0f, textureIndex, 0.5},
    //     {x + 1, y, z + 1, 1.0f, 1.0f, textureIndex, 0.5},
    //     {x,     y, z + 1, 0.0f, 1.0f, textureIndex, 0.5},
    // };
    // AddFace(face);
    constexpr float textureIndex = 0.0f;
    constexpr float faceBrightness = 0.5f;

    // Down = -Y
    //
    // v0 = (x,   y, z)     => -X, -Z
    // v1 = (x+1, y, z)     => +X, -Z
    // v2 = (x+1, y, z+1)   => +X, +Z
    // v3 = (x,   y, z+1)   => -X, +Z

    float b0 = CalculateVertexBrightness(
        map, x, y, z,
        {0, -1, 0},     // normal = -Y
        {-1, 0, 0},     // -X
        {0, 0, -1},     // -Z
        faceBrightness
    );

    float b1 = CalculateVertexBrightness(
        map, x, y, z,
        {0, -1, 0},
        {1, 0, 0},      // +X
        {0, 0, -1},     // -Z
        faceBrightness
    );

    float b2 = CalculateVertexBrightness(
        map, x, y, z,
        {0, -1, 0},
        {1, 0, 0},      // +X
        {0, 0, 1},      // +Z
        faceBrightness
    );

    float b3 = CalculateVertexBrightness(
        map, x, y, z,
        {0, -1, 0},
        {-1, 0, 0},     // -X
        {0, 0, 1},      // +Z
        faceBrightness
    );

    CubeVertex face[] = {
        {x,     y, z,     0.0f, 0.0f, textureIndex, b0},
        {x + 1, y, z,     1.0f, 0.0f, textureIndex, b1},
        {x + 1, y, z + 1, 1.0f, 1.0f, textureIndex, b2},
        {x,     y, z + 1, 0.0f, 1.0f, textureIndex, b3},
    };

    AddFace(face);
}

int Terrain::VertexAO(bool side1, bool side2, bool corner)
{
    if (side1 && side2)
    {
        return 0;
    }

    return 3 - static_cast<int>(side1) - static_cast<int>(side2) - static_cast<int>(corner);
}

float Terrain::AOToBrightness(int ao)
{
    static constexpr float brightness[] =
    {
        0.45f,  // AO 0
        0.65f,  // AO 1
        0.82f,  // AO 2
        1.00f   // AO 3
    };

    return brightness[ao];
}

float Terrain::CalculateVertexBrightness(
    const Map& map,
    int x,
    int y,
    int z,
    const glm::ivec3& normal,
    const glm::ivec3& sideA,
    const glm::ivec3& sideB,
    float faceBrightness
)
{
    const bool s1 = IsSolid(
        map,
        x + normal.x + sideA.x,
        y + normal.y + sideA.y,
        z + normal.z + sideA.z
    );

    const bool s2 = IsSolid(
        map,
        x + normal.x + sideB.x,
        y + normal.y + sideB.y,
        z + normal.z + sideB.z
    );

    const bool corner = IsSolid(
        map,
        x + normal.x + sideA.x + sideB.x,
        y + normal.y + sideA.y + sideB.y,
        z + normal.z + sideA.z + sideB.z
    );

    int ao;

    if (s1 && s2)
    {
        ao = 0;
    }
    else
    {
        ao = 3 - static_cast<int>(s1) - static_cast<int>(s2) - static_cast<int>(corner);
    }

    return faceBrightness * AOToBrightness(ao);
}