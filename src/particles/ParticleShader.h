#pragma once

#include "../Headers.h"
#include "../shaders/ShaderProgram.h"

class ParticleShader : public ShaderProgram {
public:
	ParticleShader();

	void getAllUniformLocations();
	void bindAttributes();
	void loadModelViewMatrix(glm::mat4 modelView);
	void loadProjectionMatrix(glm::mat4 projectionMatrix);

private:
	unsigned int location_modelViewMatrix;
	unsigned int location_projectionMatrix;

};