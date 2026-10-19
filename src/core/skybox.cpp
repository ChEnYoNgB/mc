#include "skybox.h"

SkyBox::SkyBox()
{
    vertices_nums = vertices.size();
    indices_nums = indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices_nums * sizeof(CubeVertex)), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices_nums * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SkyBox::Draw(ShaderManager& shaders, Camera& camera)
{
    auto& shader = shaders.Get("skybox");
    shader.Use();
    shader.SetMat4("view", glm::mat4(glm::mat3(camera.GetView())));
    shader.SetMat4("projection", camera.GetProjection());

    glBindVertexArray(VAO);
    if(EBO != 0)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_nums), GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_nums));
    }
    glBindVertexArray(0);
}