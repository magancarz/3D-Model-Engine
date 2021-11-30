#include "ParticleShader.h"

ParticleShader::ParticleShader()
	: ShaderProgram("res/shaders/particleVert.shader", "res/shaders/particleFrag.shader") {
	getAllUniformLocations();
	bindAttributes();
}

void ParticleShader::getAllUniformLocations() {
	location_numberOfRows = getUniformLocation("numberOfRows");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
}

void ParticleShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "modelView");
	bindAttribute(2, "texOffsets");
	bindAttribute(3, "blendFactor");
}

void ParticleShader::loadNumberOfRows(float numberOfRows) {
	loadFloat(location_numberOfRows, numberOfRows);
}

void ParticleShader::loadProjectionMatrix(glm::mat4 projectionMatrix) {
	loadMatrix(location_projectionMatrix, projectionMatrix);
}