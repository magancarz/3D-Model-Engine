#pragma once

#include <glm/glm.hpp>

glm::mat4 createTransformationMatrix(glm::vec2 translation, glm::vec2 scale);
glm::mat4 createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale);
float barycentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
float random();