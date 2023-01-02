#include "Maths.h"

#include <glm/gtc/type_ptr.hpp>

glm::mat4 createTransformationMatrix(glm::vec2 translation, glm::vec2 scale) {
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, glm::vec3(translation, 0.0f));
	matrix = glm::scale(matrix, glm::vec3(scale, 1.0f));

	return matrix;
}

glm::mat4 createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale) {
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), translation);
	matrix = glm::rotate(matrix, glm::radians(rx), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(ry), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rz), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

	return matrix;
}

float barycentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float random() {
	int r = rand();
	float f = (float) r / RAND_MAX;
	return f;
}