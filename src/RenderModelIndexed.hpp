#ifndef RENDERMODELINDEXED_HPP
#define RENDERMODELINDEXED_HPP

#include "Model.hpp"
#include <glm/glm.hpp>

class RenderModelIndexed : public Model {
    GLuint vertexBuffer, normalBuffer, UVBuffer, textureBuffer, elementBuffer;
protected:
    std::vector<glm::vec3> vertices_indexed;
    std::vector<glm::vec3> normals_indexed;
    std::vector<glm::vec2> UVs_indexed;
    std::vector<unsigned int> indices;
public:
    RenderModelIndexed() = default;
    RenderModelIndexed(const std::string& file_name);
    void render(GLuint vertex_attrib, GLuint normal_attrib, GLuint UV_attrib, GLuint tex_attrib);
};
#endif
