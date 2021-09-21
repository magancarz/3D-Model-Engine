#pragma once

#include "../Headers.h"

#include "../entities/Camera.h"

glm::mat4 createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale);

glm::mat4 createViewMatrix(Camera& camera);