#include "Maths.h"

glm::mat4 createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale) {
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), translation);
	matrix = glm::rotate(matrix, glm::radians(rx), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(ry), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rz), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

	return matrix;
}

glm::mat4 createViewMatrix(Camera& camera) {
	glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(camera.getPitch()), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(camera.getYaw()), glm::vec3(0, 1, 0));
	glm::vec3 translate = camera.getPosition();
	view = glm::translate(view, glm::vec3(-translate.x, -translate.y, -translate.z));

	return view;
}