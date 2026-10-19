#include "glass.h"

Glass::Glass(const std::vector<Vertex>& vertices): Geometry(vertices)
{

};

Glass::Glass(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices): Geometry(vertices, indices)
{

};

void Glass::Draw()
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
        SetUniformInt("image_index", 0);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(6), GL_UNSIGNED_INT, 0);
        SetUniformInt("image_index", 1);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(24), GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
        SetUniformInt("image_index", 2);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(6), GL_UNSIGNED_INT, (void*)(30 * sizeof(unsigned int)));
    }else{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_nums));
    }
    glBindVertexArray(0);
}