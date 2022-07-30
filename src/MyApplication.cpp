/**
 * MyApplication.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */
#include "MyApplication.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <vector>

#include "asset.hpp"
#include "glError.hpp"

#include "parse_stl.h"
#include "controls.hpp"
#include "vboindexer.hpp"
#include "Model.hpp"
#include <stdlib.h>
#include "png_loader.hpp"
#include "RenderModel.hpp"

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
};

std::vector<stl::triangle> parse_stl(const std::string& file_name) {
    auto info = stl::parse_stl(file_name);
    return info.triangles;
}

void load_model(Model& model) {
    const std::vector<std::string> parts({"/JD_SubTool1.stl","/JD_SubTool2.stl","/JD_SubTool3.stl","/JD_SubTool4.stl","/JD_SubTool5.stl","/JD_SubTool6.stl","/JD_SubTool7.stl","/JD_SubTool8.stl","/JD_SubTool9.stl","/JD_SubTool10.stl","/JD_SubTool11.stl","/JD_SubTool12.stl"});
    for (auto& part : parts) {
        model.addSTL(std::string(STL_DIR) + part);
    }
}
void load_model(std::vector<VertexType>& vertices, std::vector<unsigned short>& indices) {
    const std::vector<std::string> parts({"/JD_SubTool1.stl","/JD_SubTool2.stl","/JD_SubTool3.stl","/JD_SubTool4.stl","/JD_SubTool5.stl","/JD_SubTool6.stl","/JD_SubTool7.stl","/JD_SubTool8.stl","/JD_SubTool9.stl","/JD_SubTool10.stl","/JD_SubTool11.stl","/JD_SubTool12.stl"});
  const glm::vec4 color(0.8, 0.9, 0.9, 1);
    for (auto& part : parts) {
        std::string file_dir(STL_DIR);
        std::vector<stl::triangle> triangles = parse_stl(file_dir + part);
        std::vector<unsigned short> indices_sub;
        std::vector<glm::vec3> out_vertices, out_normals;
        indexVBO(triangles, indices_sub, out_vertices, out_normals);
        int size = out_vertices.size();

        for (int i = 0; i < size; i++) {
            vertices.push_back({out_vertices[i], out_normals[i], color});
        }
        for (int i = 0; i < indices_sub.size(); i++) {
            indices.push_back(indices_sub[i]);
        }
        std::cout << "Loaded: " << part << std::endl;
    }
}

MyApplication::MyApplication()
    : Application(),
      vertexShader(SHADER_DIR "/StandardShading.vertexshader", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/StandardShading.fragmentshader", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}) {
  glCheckError(__FILE__, __LINE__);

  const char* glsl_version = "#version 150";
  setWindow(getWindow());
  // creation of the mesh ------------------------------------------------------
  const std::vector<std::string> names = {"/boob", "/hair"};
  for (auto& name : names) {
      models.push_back(RenderModel(name));
  }
  // creation of the vertex array buffer----------------------------------------


  // vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /* models.push_back(RenderModel(names[0])); */
  // vao end
  glBindVertexArray(0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(getWindow(), true);
  ImGui_ImplOpenGL3_Init(glsl_version);

}

void MyApplication::loop() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  const float radius = 100.0;
  /* float t = getTime(); */
  // set matrix : projection + view
  /* projection = glm::perspective(float(2.0 * atan(getHeight() / 1920.f)), */
  /*                               getWindowRatio(), 0.1f, 100.f); */
  computeMatricesFromInputs();
  projection = getProjectionMatrix();
  glm::mat4 scale(0.1);
  view = getViewMatrix();
  glm::mat4 MVP = projection * view * scale;

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0, 0.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  Texture_ID = shaderProgram.uniform("myTextureSampler");
  GLuint LightID = shaderProgram.uniform("LightPosition_worldspace");
  /* glActiveTexture(GL_TEXTURE0); */
  /* glBindTexture(GL_TEXTURE_2D, Texture); */
  /* glUniform1i(Texture_ID, 0); */

  // send uniforms
  shaderProgram.setUniform("MVP", MVP);
  shaderProgram.setUniform("V", view);
  shaderProgram.setUniform("M", scale);
  glm::vec3 lightPos = glm::vec3(0,-10,0);
  glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

  /* glCheckError(__FILE__, __LINE__); */

  glBindVertexArray(vao);

  GLuint position_id = shaderProgram.attribute("vertexPosition_modelspace");
  GLuint UV_id= shaderProgram.attribute("vertexUV");
  GLuint normal_id = shaderProgram.attribute("vertexNormal_modelspace");
  for (auto& model : models)
      model.render(position_id, normal_id, UV_id, Texture_ID);
  
  /* glCheckError(__FILE__, __LINE__); */
  /* glDrawArrays(GL_TRIANGLES, 0, size); */
  /* glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, (void*)0); */
  glBindVertexArray(0);

  shaderProgram.unuse();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  /* ImGui::ShowDemoWindow(); */
  {
      ImGui::SetNextWindowPos({0,0});
      ImGui::SetNextWindowSize({0.0, 0.0});
      ImGui::Begin("Camera info");
      glm::vec3 position = getPosition();
      ImGui::Text("Camera position: %f, %f, %f", position.x, position.y, position.z);
      ImGui::Text("Horizontal Angle: %f", getHorizontalAngle()*glm::one_over_pi<float>()*180.0);
      ImGui::Text("Vertical Angle: %f", getVerticalAngle()*glm::one_over_pi<float>()*180.0);
      ImGui::Text("FPS: %f", 1/getFrameDeltaTime());
      ImGui::End();
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
