#include "shadermanager.h"

Shader& ShaderManager::Add(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
{
    auto shader = std::make_unique<Shader>(vertexSource, fragmentSource);
    Shader& result = *shader;
    shaders.emplace(name, std::move(shader));
    return result;
}

Shader& ShaderManager::Add(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource, const std::string& image_path)
{
    auto shader = std::make_unique<Shader>(vertexSource, fragmentSource);
    Shader& result = *shader;
    result.SetTexture(image_path);
    shaders.emplace(name, std::move(shader));
    return result;
}

Shader& ShaderManager::Get(const std::string& name)
{
    return *shaders.at(name);
}