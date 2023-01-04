#include "Maths.h"

#include <random>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 create_transformation_matrix(const glm::vec2& translation, const glm::vec2& scale) {
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, glm::vec3(translation, 0.0f));
	matrix = glm::scale(matrix, glm::vec3(scale, 1.0f));

	return matrix;
}

glm::mat4 create_transformation_matrix(const glm::vec3& translation, const float rx, const float ry, const float rz, const float scale) {
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), translation);
	matrix = glm::rotate(matrix, glm::radians(rx), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(ry), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rz), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

	return matrix;
}

double barycentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) {
	const double det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	const double l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	const double l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	const double l3 = 1.0 - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float random() {
	std::uniform_real_distribution<float> int_distribution(0, RAND_MAX);
	std::mt19937 random_number_engine;
    auto generator = [&]() { return int_distribution(random_number_engine); };

	return generator();
}