/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_MYAPPLICATION
#define OPENGL_CMAKE_SKELETON_MYAPPLICATION

#include "Application.hpp"
#include "Shader.hpp"
#include "RenderModel.hpp"

class MyApplication : public Application {
 public:
  MyApplication();

 protected:
  virtual void loop();
 private:
    int size = 100;

  // shader
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbos[3], Texture, Texture_ID;
  std::vector<RenderModel> models;
};

#endif  // OPENGL_CMAKE_SKELETON_MYAPPLICATION
