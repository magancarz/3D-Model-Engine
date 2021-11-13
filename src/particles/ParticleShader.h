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
	void loadTextureCoordInfo(glm::vec2 offset1, glm::vec2 offset2, float numRows, float blend);

private:
	unsigned int location_modelViewMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_texOffset1;
	unsigned int location_texOffset2;
	unsigned int location_texCoordInfo;

};