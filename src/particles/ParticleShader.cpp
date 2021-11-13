#include "ParticleShader.h"

ParticleShader::ParticleShader()
	: ShaderProgram("res/shaders/particleVert.shader", "res/shaders/particleFrag.shader") {
	getAllUniformLocations();
	bindAttributes();
}

void ParticleShader::getAllUniformLocations() {
	location_modelViewMatrix = getUniformLocation("modelViewMatrix");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_texOffset1 = getUniformLocation("texOffset1");
	location_texOffset2 = getUniformLocation("texOffset2");
	location_texCoordInfo = getUniformLocation("texCoordInfo");
}

void ParticleShader::bindAttributes() {
	bindAttribute(0, "position");
}

void ParticleShader::loadModelViewMatrix(glm::mat4 modelViewMatrix) {
	loadMatrix(location_modelViewMatrix, modelViewMatrix);
}

void ParticleShader::loadProjectionMatrix(glm::mat4 projectionMatrix) {
	loadMatrix(location_projectionMatrix, projectionMatrix);
}

void ParticleShader::loadTextureCoordInfo(glm::vec2 offset1, glm::vec2 offset2, float numRows, float blend) {
	loadVector2f(location_texOffset1, offset1);
	loadVector2f(location_texOffset2, offset2);
	loadVector2f(location_texCoordInfo, glm::vec2(numRows, blend));
}