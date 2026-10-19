#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "geometry.h"

Geometry::Geometry(const std::vector<Vertex>& vertices)
{
    vertices_nums = vertices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_nums * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
};

Geometry::Geometry(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    vertices_nums = vertices.size();
    indices_nums = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_nums * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices_nums * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
};

void Geometry::Draw()
{
    glUseProgram(shader);
    if(texture !=0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    glBindVertexArray(VAO);
    if(EBO != 0)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_nums), GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_nums));
    }
    glBindVertexArray(0);
}

void Geometry::SetUniformMatrix4fv(const std::string& uniform_name, const glm::mat4& data)
{
    int modelLoc = glGetUniformLocation(shader, uniform_name.c_str());
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(data));
}

void Geometry::SetUniformInt(const std::string& uniform_name, int data)
{
    int modelLoc = glGetUniformLocation(shader, uniform_name.c_str());
    glUniform1i(modelLoc, data);
}

void Geometry::SetShader(unsigned int shader)
{
    this->shader = shader;
}

void Geometry::SetTexture(const std::string image_path)
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

Geometry::~Geometry()
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