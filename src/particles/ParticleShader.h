#pragma once

#include "../Headers.h"
#include "../shaders/ShaderProgram.h"

class ParticleShader : public ShaderProgram {
public:
	ParticleShader();

	void getAllUniformLocations();
	void bindAttributes();
	void loadNumberOfRows(float numberOfRows);
	void loadProjectionMatrix(glm::mat4 projectionMatrix);

private:
	unsigned int location_numberOfRows;
	unsigned int location_projectionMatrix;

};