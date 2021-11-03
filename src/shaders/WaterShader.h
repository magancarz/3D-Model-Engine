#pragma once

#include "../entities/Camera.h"
#include "ShaderProgram.h"

class WaterShader : public ShaderProgram {
public:
	WaterShader();

	void loadProjectionMatrix(glm::mat4 projection);
	void loadViewMatrix(Camera& camera);
	void loadModelMatrix(glm::mat4 modelMatrix);
	void connectTextureUnits();
private:
	void bindAttributes();
	void getAllUniformLocations();

	unsigned int location_modelMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_reflectionTexture;
	unsigned int location_refractionTexture;

};