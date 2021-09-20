#include "StaticShader.h"

StaticShader::StaticShader()
	: ShaderProgram("res/shaders/vert.shader", "res/shaders/frag.shader" ), location_transformationMatrix(0) {
	bindAttributes();
	getAllUniformLocations();
}

void StaticShader::loadTransformationMatrix(glm::mat4 matrix) {
	loadMatrix(location_transformationMatrix, matrix);
}

void StaticShader::loadProjectionMatrix(glm::mat4 matrix) {
	loadMatrix(location_projectionMatrix, matrix);
}

void StaticShader::loadViewMatrix(Camera& camera) {
	glm::mat4 view = glm::lookAt(
		glm::vec3(1.2f, 1.2f, 1.2f) * 2.0f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	loadMatrix(location_viewMatrix, view);
}

void StaticShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
}

void StaticShader::getAllUniformLocations() {
	location_transformationMatrix = getUniformLocation("model");
	location_projectionMatrix = getUniformLocation("proj");
	location_viewMatrix = getUniformLocation("view");
}