#include "WaterShader.h"

void WaterShader::bindAttributes() {
	bindAttribute(0, "position");
}

void WaterShader::getAllUniformLocations() {
	location_projectionMatrix = getUniformLocation("proj");
	location_viewMatrix = getUniformLocation("view");
	location_modelMatrix = getUniformLocation("model");
}

void WaterShader::loadProjectionMatrix(glm::mat4 projection) {
	loadMatrix(location_projectionMatrix, projection);
}

void WaterShader::loadViewMatrix(Camera& camera) {
	glm::mat4 viewMatrix = camera.getView();
	loadMatrix(location_viewMatrix, viewMatrix);
}

void WaterShader::loadModelMatrix(glm::mat4 modelMatrix) {
	loadMatrix(location_modelMatrix, modelMatrix);
}