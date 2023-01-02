#include "WaterShader.h"

WaterShader::WaterShader()
	: ShaderProgram("res/shaders/waterVert.glsl", "res/shaders/waterFrag.glsl") {
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
	location_normalMap = getUniformLocation("normalMap");
	location_depthMap = getUniformLocation("depthMap");
	location_moveFactor = getUniformLocation("moveFactor");
	location_lightPosition = getUniformLocation("lightPosition");
	location_lightColor = getUniformLocation("lightColor");
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

void WaterShader::loadLight(Light& sun) {
	loadVector3f(location_lightPosition, sun.getPosition());
	loadVector3f(location_lightColor, sun.getColor());
}

void WaterShader::connectTextureUnits() {
	loadInt(location_reflectionTexture, 0);
	loadInt(location_refractionTexture, 1);
	loadInt(location_dudvMap, 2);
	loadInt(location_normalMap, 3);
	loadInt(location_depthMap, 4);
}