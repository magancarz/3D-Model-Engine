#pragma once

#include "../entities/Camera.h"
#include "ShaderProgram.h"

class WaterShader : public ShaderProgram {
public:
	WaterShader()
		: ShaderProgram("res/shaders/waterFrag.shader", "res/shaders/waterVert.shader") {}

	void loadProjectionMatrix(glm::mat4 projection);
	void loadViewMatrix(Camera& camera);
	void loadModelMatrix(glm::mat4 modelMatrix);
private:
	void bindAttributes();
	void getAllUniformLocations();

	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
};