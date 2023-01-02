#pragma once

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "shaders/ShaderProgram.h"

class TerrainShader : public ShaderProgram {
public:
	TerrainShader();

	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(Camera& camera);
	void loadLights(std::vector<Light*>& lights);
	void loadShineVariables(float shineDamper, float reflectivity);
	void loadSkyColor(float r, float g, float b);
	void loadClipPlane(glm::vec4 plane);
	void loadToShadowMapSpaceMatrix(glm::mat4 toShadowMapSpace);

	void connectTextureUnits();
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

	unsigned int location_skyColor;

	unsigned int location_backgroundTexture;
	unsigned int location_rTexture;
	unsigned int location_gTexture;
	unsigned int location_bTexture;
	unsigned int location_blendMap;

	unsigned int location_plane;

	unsigned int location_toShadowMapSpace;
	unsigned int location_shadowMap;
};
