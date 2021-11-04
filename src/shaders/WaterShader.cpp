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
	location_dudvMap = getUniformLocation("dudvMap");
	location_moveFactor = getUniformLocation("moveFactor");
	location_cameraPosition = getUniformLocation("cameraPosition");
}

void WaterShader::loadProjectionMatrix(glm::mat4 projection) {
	loadMatrix(location_projectionMatrix, projection);
}

void WaterShader::loadViewMatrix(Camera& camera) {
	glm::mat4 viewMatrix = camera.getView();
	loadMatrix(location_viewMatrix, viewMatrix);
	loadVector3f(location_cameraPosition, camera.getPosition());
}

void WaterShader::loadModelMatrix(glm::mat4 modelMatrix) {
	loadMatrix(location_modelMatrix, modelMatrix);
}

void WaterShader::loadMoveFactor(float value) {
	loadFloat(location_moveFactor, value);
}

void WaterShader::connectTextureUnits() {
	loadInt(location_reflectionTexture, 0);
	loadInt(location_refractionTexture, 1);
	loadInt(location_dudvMap, 2);
}