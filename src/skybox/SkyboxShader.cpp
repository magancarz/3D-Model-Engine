#include "SkyboxShader.h"

SkyboxShader::SkyboxShader() : ShaderProgram("res/shaders/skyboxVertShader.glsl", "res/shaders/skyboxFragShader.glsl") {
	bindAttributes();
	getAllUniformLocations();
}

void SkyboxShader::bindAttributes() {
	bindAttribute(0, "position");
}

void SkyboxShader::getAllUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_fogColor = getUniformLocation("fogColor");
	location_cubeMap = getUniformLocation("cubeMap");
	location_cubeMap2 = getUniformLocation("cubeMap2");
	location_blendFactor = getUniformLocation("blendFactor");
}

void SkyboxShader::loadProjectionMatrix(glm::mat4& matrix) {
	loadMatrix(location_projectionMatrix, matrix);
}

void SkyboxShader::loadViewMatrix(Camera& camera) {
	glm::mat4 view = camera.getView();
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	rotation += ROTATE_SPEED * display.getFrameTimeSeconds();
	view = glm::rotate(view, glm::radians(rotation), glm::vec3(0, 1, 0));
	loadMatrix(location_viewMatrix, view);
}

void SkyboxShader::loadFogColor(GLfloat r, GLfloat g, GLfloat b) {
	glm::vec3 vec(r, g, b);
	loadVector3f(location_fogColor, vec);
}

void SkyboxShader::connectTextureUnits() {
	loadInt(location_cubeMap, 0);
	loadInt(location_cubeMap2, 1);
}

void SkyboxShader::loadBlendFactor(GLfloat blend) {
	loadFloat(location_blendFactor, blend);
}