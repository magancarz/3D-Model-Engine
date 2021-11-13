#include "ParticleShader.h"

ParticleShader::ParticleShader()
	: ShaderProgram("res/shaders/particleVert.shader", "res/shaders/particleFrag.shader") {
	getAllUniformLocations();
	bindAttributes();
}

void ParticleShader::getAllUniformLocations() {
	location_modelViewMatrix = getUniformLocation("modelViewMatrix");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
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