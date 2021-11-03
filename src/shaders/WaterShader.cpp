#include "WaterShader.h"

WaterShader::WaterShader()
	: ShaderProgram("res/shaders/waterVert.shader", "res/shaders/waterFrag.shader") {
	bindAttributes();
	getAllUniformLocations();
}

void WaterShader::bindAttributes() {
	bindAttribute(0, "position");
}

void WaterShader::getAllUniformLocations() {
	location_projectionMatrix = getUniformLocation("proj");
	location_viewMatrix = getUniformLocation("view");
	location_modelMatrix = getUniformLocation("model");
	location_reflectionTexture = getUniformLocation("reflectionTexture");
	location_refractionTexture = getUniformLocation("refractionTexture");
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

void WaterShader::connectTextureUnits() {
	loadInt(location_reflectionTexture, 0);
	loadInt(location_refractionTexture, 1);
}