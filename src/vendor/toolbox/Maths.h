#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../entities/Camera.h"

glm::mat4 createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale);

glm::mat4 createViewMatrix(Camera& camera);