#ifndef RENDERMODEL_HPP
#define RENDERMODEL_HPP

#include "Model.hpp"
#include <glm/glm.hpp>

class RenderModel : public Model {
    GLuint vertexBuffer, normalBuffer, UVBuffer, textureBuffer;
public:
    RenderModel() = default;
    RenderModel(const std::string& fileName);
    void render(GLuint vertex_attrib, GLuint normal_attrib, GLuint UV_attrib, GLuint tex_attrib);
};


#endif
