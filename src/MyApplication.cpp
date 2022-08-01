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
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "asset.hpp"
#include "glError.hpp"

#include "parse_stl.h"
#include "controls.hpp"
#include "vboindexer.hpp"
#include "Model.hpp"
#include <stdlib.h>
#include "png_loader.hpp"
#include "RenderModel.hpp"
#include "RenderModelIndexed.hpp"
#include "os/fileSystem.hpp"

float scale_factor = 1.0;
static std::vector<std::string> model_names;
static bool model_names_checked[1024] = {false};
static std::vector<bool> model_names_loaded;

const std::vector<std::string> getDirOBJs() {
    auto dirContents = getDirContents(OBJ_DIR);
    std::vector<std::string> res;
    for (auto& entry : dirContents) {
        if (entry.find(".obj") != std::string::npos)
            res.push_back(entry.substr(0, entry.find(".obj")));
    }
    return res;
}

void load_model_names() {
    auto obj_dir_entries = getDirOBJs();
    for (auto& entry: obj_dir_entries) {
        model_names.push_back(entry);
        /* model_names_checked.push_back(false); */
        model_names_loaded.push_back(false);
    }
}

void load_model(std::vector<RenderModelIndexed>& models) {
    for (int i = 0; i < model_names.size(); i++) {
        if (model_names_checked[i] && model_names_loaded[i] == false) {
            models.push_back(RenderModelIndexed("/" + model_names[i]));
            model_names_loaded[i] = true;
        }
    }
}

bool model_menu_checked(const RenderModelIndexed& model) {
    for (int i = 0; i < model_names.size(); i++) {
        if (model.getName().find(model_names[i]) != std::string::npos && model_names_checked[i])
            return true;
    }
    return false;
}

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

MyApplication::MyApplication()
    : Application(),
      vertexShader(SHADER_DIR "/StandardShading.vertexshader", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/StandardShading.fragmentshader", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}) {
  glCheckError(__FILE__, __LINE__);

  const char* glsl_version = "#version 150";
  setWindow(getWindow());
  // creation of the mesh ------------------------------------------------------
  /* const std::vector<std::string> names = {"/boob", "/hair"}; */
  /* for (auto& name : names) { */
  /*     models.push_back(RenderModel(name)); */
  /* } */
  // creation of the vertex array buffer----------------------------------------

  // vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vao end
  glBindVertexArray(0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(getWindow(), true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  load_model_names();
}

void MyApplication::loop() {
    load_model(models);
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  computeMatricesFromInputs();
  projection = getProjectionMatrix();
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), {scale_factor, scale_factor, scale_factor});
  view = getViewMatrix();
  glm::mat4 MVP = projection * view * scale;

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  GLuint Texture_ID = shaderProgram.uniform("myTextureSampler");
  GLuint LightID = shaderProgram.uniform("LightPosition_worldspace");

  // send uniforms
  shaderProgram.setUniform("MVP", MVP);
  shaderProgram.setUniform("V", view);
  shaderProgram.setUniform("M", scale);
  glm::vec3 lightPos = glm::vec3(0,-10,0);
  glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


  glBindVertexArray(vao);

  GLuint position_id = shaderProgram.attribute("vertexPosition_modelspace");
  GLuint UV_id= shaderProgram.attribute("vertexUV");
  GLuint normal_id = shaderProgram.attribute("vertexNormal_modelspace");
  // Actually drawing
  for (auto& model : models) {
      if (model_menu_checked(model))
        model.render(position_id, normal_id, UV_id, Texture_ID);
  }
  
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
      /* ImGui::SetNextWindowPos({0,0}); */
      ImGui::SetNextWindowSize({0.0, 0.0});
      ImGui::Begin("Settings");
      ImGui::DragFloat("Scale", &scale_factor, 0.1, 0.0, 1.0);
      for(int i = 0; i < model_names.size(); i++) {
          ImGui::Checkbox(model_names[i].c_str(), &model_names_checked[i]);
      }
      ImGui::End();
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
