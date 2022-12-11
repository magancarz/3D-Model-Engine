#include "StaticShader.h"

StaticShader::StaticShader()
	: ShaderProgram("res/shaders/vert.glsl", "res/shaders/frag.glsl" ), location_transformationMatrix(0) {
	bindAttributes();
	getAllUniformLocations();
}

void StaticShader::loadTransformationMatrix(glm::mat4 matrix) {
	loadMatrix(location_transformationMatrix, matrix);
}

void StaticShader::loadProjectionMatrix(glm::mat4 matrix) {
	loadMatrix(location_projectionMatrix, matrix);
}

void StaticShader::loadViewMatrix(Camera& camera) {
	glm::mat4 view = camera.getView();
	loadMatrix(location_viewMatrix, view);
}

void StaticShader::loadLights(std::vector<Light*>& lights) {
	for(size_t i = 0; i < MAX_LIGHTS; i++) {
		if(i < lights.size()) {
			loadVector3f(location_lightPosition[i], lights[i]->getPosition());
			loadVector3f(location_lightColor[i], lights[i]->getColor());
			loadVector3f(location_attenuation[i], lights[i]->getAttenuation());
		} else {
			loadVector3f(location_lightPosition[i], glm::vec3(0));
			loadVector3f(location_lightColor[i], glm::vec3(0));
			loadVector3f(location_attenuation[i], glm::vec3(1, 0, 0));
		}
	}
}

void StaticShader::loadShineVariables(float shineDamper, float reflectivity) {
	loadFloat(location_shineDamper, shineDamper);
	loadFloat(location_reflectivity, reflectivity);
}

void StaticShader::loadFakeLightingVariable(bool value) {
	loadBoolean(location_useFakeLighting, value);
}

void StaticShader::loadSkyColor(float r, float g, float b) {
	loadVector3f(location_skyColor, glm::vec3(r, g, b));
}

void StaticShader::loadNumberOfRows(float value) {
	loadFloat(location_numberOfRows, value);
}

void StaticShader::loadOffset(glm::vec2 offset) {
	loadVector2f(location_offset, offset);
}

void StaticShader::connectTextureUnits() {
	loadInt(location_modelTexture, 0);
	loadInt(location_specularMap, 1);
}

void StaticShader::loadUseSpecularMap(bool useMap) {
	loadBoolean(location_usesSpecularMap, useMap);
}

void StaticShader::loadClipPlane(glm::vec4 plane) {
	loadVector4f(location_plane, plane);
}

void StaticShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
}

void StaticShader::getAllUniformLocations() {
	//MVP calculation uniforms
	location_transformationMatrix = getUniformLocation("model");
	location_projectionMatrix = getUniformLocation("proj");
	location_viewMatrix = getUniformLocation("view");

	//Light calculation uniforms
	for(int i = 0; i < MAX_LIGHTS; i++) {
		location_lightPosition[i] = getUniformLocation("lightPosition[" + std::to_string(i) + "]");
		location_lightColor[i] = getUniformLocation("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = getUniformLocation("attenuation[" + std::to_string(i) + "]");
	}
	location_shineDamper = getUniformLocation("shineDamper");
	location_reflectivity = getUniformLocation("reflectivity");
	location_useFakeLighting = getUniformLocation("useFakeLighting");

	//Fog calculation uniform
	location_skyColor = getUniformLocation("skyColor");

	//Texture atlases uniforms
	location_numberOfRows = getUniformLocation("numberOfRows");
	location_offset = getUniformLocation("offset");

	//Clip plane
	location_plane = getUniformLocation("plane");

	//Specular map uniforms
	location_specularMap = getUniformLocation("specularSampler");
	location_modelTexture = getUniformLocation("textureSampler");
	location_usesSpecularMap = getUniformLocation("usesSpecularMap");
}