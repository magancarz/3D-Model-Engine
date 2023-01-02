#include "TerrainShader.h"

TerrainShader::TerrainShader()
	: ShaderProgram("res/shaders/terrainVert.glsl", "res/shaders/terrainFrag.glsl"), location_transformationMatrix(0) {
	bind_attributes();
	get_all_uniform_locations();
}

void TerrainShader::loadTransformationMatrix(glm::mat4 matrix) {
	load_matrix(location_transformationMatrix, matrix);
}

void TerrainShader::loadProjectionMatrix(glm::mat4 matrix) {
	load_matrix(location_projectionMatrix, matrix);
}

void TerrainShader::loadViewMatrix(Camera& camera) {
	glm::mat4 view = camera.getView();
	load_matrix(location_viewMatrix, view);
}

void TerrainShader::loadLights(const std::vector<std::shared_ptr<Light>>& lights) {
	for(size_t i = 0; i < MAX_LIGHTS; i++) {
		if(i < lights.size()) {
			load_vector3_f(location_lightPosition[i], lights[i]->get_position());
			load_vector3_f(location_lightColor[i], lights[i]->get_color());
			load_vector3_f(location_attenuation[i], lights[i]->get_attenuation());
		} else {
			load_vector3_f(location_lightPosition[i], glm::vec3(0));
			load_vector3_f(location_lightColor[i], glm::vec3(0));
			load_vector3_f(location_attenuation[i], glm::vec3(1, 0, 0));
		}
	}
}

void TerrainShader::loadShineVariables(float shineDamper, float reflectivity) {
	load_float(location_shineDamper, shineDamper);
	load_float(location_reflectivity, reflectivity);
}

void TerrainShader::loadSkyColor(float r, float g, float b) {
	load_vector3_f(location_skyColor, glm::vec3(r, g, b));
}

void TerrainShader::loadClipPlane(glm::vec4 plane) {
	load_vector4_f(location_plane, plane);
}

void TerrainShader::loadToShadowMapSpaceMatrix(glm::mat4 toShadowMapSpace) {
	load_matrix(location_toShadowMapSpace, toShadowMapSpace);
}

void TerrainShader::bind_attributes() {
	bind_attribute(0, "position");
	bind_attribute(1, "textureCoords");
	bind_attribute(2, "normal");
}

void TerrainShader::get_all_uniform_locations() {
	//MVP calculation uniforms
	location_transformationMatrix = get_uniform_location("model");
	location_projectionMatrix = get_uniform_location("proj");
	location_viewMatrix = get_uniform_location("view");

	//Light calculation uniforms
	for(int i = 0; i < MAX_LIGHTS; i++) {
		location_lightPosition[i] = get_uniform_location("lightPosition[" + std::to_string(i) + "]");
		location_lightColor[i] = get_uniform_location("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = get_uniform_location("attenuation[" + std::to_string(i) + "]");
	}
	location_shineDamper = get_uniform_location("shineDamper");
	location_reflectivity = get_uniform_location("reflectivity");

	//Fog calculation uniforms
	location_skyColor = get_uniform_location("skyColor");

	//Texture samplers
	location_backgroundTexture = get_uniform_location("backgroundTexture");
	location_rTexture = get_uniform_location("rTexture");
	location_gTexture = get_uniform_location("gTexture");
	location_bTexture = get_uniform_location("bTexture");
	location_blendMap = get_uniform_location("blendMap");

	//Clip plane
	location_plane = get_uniform_location("plane");

	//Shadows
	location_toShadowMapSpace = get_uniform_location("toShadowMapSpace");
	location_shadowMap = get_uniform_location("shadowMap");
}

void TerrainShader::connectTextureUnits() {
	load_int(location_backgroundTexture, 0);
	load_int(location_rTexture, 1);
	load_int(location_gTexture, 2);
	load_int(location_bTexture, 3);
	load_int(location_blendMap, 4);
	load_int(location_shadowMap, 5);
}