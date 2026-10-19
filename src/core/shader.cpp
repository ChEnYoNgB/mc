#include "stb_image.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    int  success;
    char infoLog[512];
    unsigned int vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    unsigned int frameShader = 0;
    frameShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = fragmentShaderSource.c_str();
    glShaderSource(frameShader, 1, &fragmentSource, NULL);
    glCompileShader(frameShader);
    glGetShaderiv(frameShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(frameShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, frameShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(frameShader);
}

void Shader::Use()const
{
    glUseProgram(shaderProgram);
    
    if(texture != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
    int modelLoc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    int modelLoc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3fv(modelLoc, 1, glm::value_ptr(value));
}

void Shader::SetFloat(const std::string& name, float value) const
{
    int modelLoc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(modelLoc, value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    int modelLoc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(modelLoc, value);
}

void Shader::SetTexture(const std::string& image_path)
{
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
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

Shader::~Shader()
{
    if (shaderProgram != 0)
        glDeleteProgram(shaderProgram);
}