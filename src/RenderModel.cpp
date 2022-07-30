#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "asset.hpp"
#include "Model.hpp"
#include "RenderModel.hpp"

RenderModel::RenderModel(const std::string& fileName) : Model(fileName) {
        loadOBJ(std::string(OBJ_DIR) + fileName + ".obj");
        loadTexture(std::string(TEXTURE_DIR) + fileName + ".png");

        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &normalBuffer);
        glGenBuffers(1, &UVBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);        
        glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
        glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);        

        glGenTextures(1, &textureBuffer);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.x, texture.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
        glGenerateMipmap(GL_TEXTURE_2D); // Unavailable in OpenGL 2.1, use gluBuild2DMipmaps() instead

        glBindTexture(GL_TEXTURE_2D, 0);
}
void RenderModel::render(GLuint vertex_attrib, GLuint normal_attrib, GLuint UV_attrib, GLuint tex_attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glEnableVertexAttribArray(vertex_attrib);
        std::cout << "Render vertex: " << vertex_attrib << std::endl;
        glVertexAttribPointer(vertex_attrib, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(0));
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glEnableVertexAttribArray(normal_attrib);
        std::cout << "Render normal: " << normal_attrib<< std::endl;
        glVertexAttribPointer(normal_attrib, 3, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(0));
        glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
        glEnableVertexAttribArray(UV_attrib);
        std::cout << "Render UV: " << UV_attrib << std::endl;
        glVertexAttribPointer(UV_attrib, 2, GL_FLOAT, false, 0, reinterpret_cast<GLvoid*>(0));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glUniform1i(tex_attrib, 0);
        std::cout << "Render Texture: " << tex_attrib << std::endl;

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glDisableVertexAttribArray(UV_attrib);
        /* glBindBuffer(GL_ARRAY_BUFFER, normalBuffer); */
        glDisableVertexAttribArray(normal_attrib);
        /* glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); */
        glDisableVertexAttribArray(vertex_attrib);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
