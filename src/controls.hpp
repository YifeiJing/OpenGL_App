#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void setWindow(GLFWwindow* window);
void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getPosition();
float getHorizontalAngle();
float getVerticalAngle();
#endif
