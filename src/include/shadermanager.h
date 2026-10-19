#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "shader.h"

class ShaderManager
{
public:
    Shader& Add(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
    Shader& Add(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource, const std::string& image_path);
    Shader& Get(const std::string& name);
private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};