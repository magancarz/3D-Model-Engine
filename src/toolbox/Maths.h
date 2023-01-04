#pragma once

#include <glm/glm.hpp>

glm::mat4 create_transformation_matrix(const glm::vec2& translation, const glm::vec2& scale);

glm::mat4 create_transformation_matrix(const glm::vec3& translation, float rx, float ry, float rz, float scale);

double barycentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);

float random();