#pragma once

#include "../Headers.h"

#include "ShaderProgram.h"
#include "../entities/Camera.h"
#include "../entities/Light.h"

class StaticShader : public ShaderProgram {
public:
	StaticShader();

	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(Camera& camera);
	void loadLight(Light& light);
	void loadShineVariables(float shineDamper, float reflectivity);
	void loadFakeLightingVariable(bool value);
	void loadSkyColor(float r, float g, float b);
	void loadNumberOfRows(float value);
	void loadOffset(glm::vec2 offset);
protected:
	void virtual bindAttributes() override;
	void virtual getAllUniformLocations() override;
private:
	const std::string VERTEX_FILE = "res/shaders/vert.shader";
	const std::string FRAGMENT_FILE = "res/shaders/frag.shader";

	unsigned int location_transformationMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;

	unsigned int location_lightPosition;
	unsigned int location_lightColor;
	unsigned int location_shineDamper;
	unsigned int location_reflectivity;
	unsigned int location_useFakeLighting;
	unsigned int location_skyColor;
	unsigned int location_numberOfRows;
	unsigned int location_offset;
};