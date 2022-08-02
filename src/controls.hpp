#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void setWindow(GLFWwindow* window);
void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getPosition();
float getHorizontalAngle();
float getVerticalAngle();
void setPosition(float x, float y, float z);
void setHorizontalAngle(float v);
void setVerticalAngle(float v);
#endif
