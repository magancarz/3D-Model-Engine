#pragma once

#include <vector>

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "shaders/ShaderProgram.h"

class NormalMappingShader : public ShaderProgram {
public:
	NormalMappingShader();

	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(Camera& camera);
	void loadLights(std::vector<Light*>& lights, glm::mat4 viewMatrix);
	void loadShineVariables(float shineDamper, float reflectivity);
	//void load_fake_lighting_variable(bool value);
	void loadSkyColor(float r, float g, float b);
	void loadNumberOfRows(int value);
	void loadOffset(float x, float y);
	void loadClipPlane(glm::vec4 plane);
	void connectTextureUnits();

private:
	void bind_attributes();
	void get_all_uniform_locations();

	glm::vec3 getEyeSpacePosition(Light* light, glm::mat4 viewMatrix);

	static constexpr int MAX_LIGHTS = 4;
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_lightPosition[MAX_LIGHTS];
	int location_lightColor[MAX_LIGHTS];
	int location_attenuation[MAX_LIGHTS];
	int location_shineDamper;
	int location_reflectivity;
	int location_useFakeLighting;
	int location_skyColor;
	int location_fogDensity;
	int location_fogGradient;
	int location_numberOfRows;
	int location_textureOffset;
	int location_clipPlane;
	int location_modelTexture;
	int location_normalMap;

};
