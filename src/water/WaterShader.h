#pragma once

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "shaders/ShaderProgram.h"

class WaterShader : public ShaderProgram {
public:
	WaterShader();

	void loadProjectionMatrix(glm::mat4 projection);
	void loadViewMatrix(Camera& camera);
	void loadModelMatrix(glm::mat4 modelMatrix);
	void loadMoveFactor(float value);
	void loadLight(Light& sun);
	void connectTextureUnits();
private:
	void bind_attributes();
	void get_all_uniform_locations();

	unsigned int location_modelMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_reflectionTexture;
	unsigned int location_refractionTexture;
	unsigned int location_dudvMap;
	unsigned int location_normalMap;
	unsigned int location_depthMap;
	unsigned int location_moveFactor;
	unsigned int location_lightPosition;
	unsigned int location_lightColor;
	unsigned int location_cameraPosition;

};
