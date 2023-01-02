#pragma once

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "shaders/ShaderProgram.h"

class StaticShader : public ShaderProgram {
public:
	StaticShader();

	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(Camera& camera);
	void loadLights(std::vector<Light*>& lights);
	void loadShineVariables(float shineDamper, float reflectivity);
	void loadFakeLightingVariable(bool value);
	void loadSkyColor(float r, float g, float b);
	void loadNumberOfRows(float value);
	void loadOffset(glm::vec2 offset);
	void loadClipPlane(glm::vec4 plane);
	void connectTextureUnits();
	void loadUseSpecularMap(bool useMap);

protected:
	void virtual bindAttributes() override;
	void virtual getAllUniformLocations() override;
private:
	const std::string VERTEX_FILE = "res/shaders/vert.shader";
	const std::string FRAGMENT_FILE = "res/shaders/frag.shader";

	static const int MAX_LIGHTS = 4;

	unsigned int location_transformationMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;

	unsigned int location_lightPosition[MAX_LIGHTS];
	unsigned int location_lightColor[MAX_LIGHTS];
	unsigned int location_attenuation[MAX_LIGHTS];
	unsigned int location_shineDamper;
	unsigned int location_reflectivity;
	unsigned int location_useFakeLighting;
	unsigned int location_skyColor;
	unsigned int location_numberOfRows;
	unsigned int location_offset;
	unsigned int location_plane;
	unsigned int location_specularMap;
	unsigned int location_modelTexture;
	unsigned int location_usesSpecularMap;
};
