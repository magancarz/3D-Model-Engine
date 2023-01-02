#include "SkyboxShader.h"

#include "toolbox/DisplayManager.h"

SkyboxShader::SkyboxShader() : ShaderProgram("res/shaders/skyboxVertShader.glsl", "res/shaders/skyboxFragShader.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void SkyboxShader::bind_attributes() {
	bind_attribute(0, "position");
}

void SkyboxShader::get_all_uniform_locations() {
	location_projectionMatrix = get_uniform_location("projectionMatrix");
	location_viewMatrix = get_uniform_location("viewMatrix");
	location_fogColor = get_uniform_location("fogColor");
	location_cubeMap = get_uniform_location("cubeMap");
	location_cubeMap2 = get_uniform_location("cubeMap2");
	location_blendFactor = get_uniform_location("blendFactor");
}

void SkyboxShader::loadProjectionMatrix(glm::mat4& matrix) {
	load_matrix(location_projectionMatrix, matrix);
}

void SkyboxShader::loadViewMatrix(Camera& camera) {
	glm::mat4 view = camera.getView();
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	rotation += ROTATE_SPEED * DisplayManager::getFrameTimeSeconds();
	view = glm::rotate(view, glm::radians(rotation), glm::vec3(0, 1, 0));
	load_matrix(location_viewMatrix, view);
}

void SkyboxShader::loadFogColor(GLfloat r, GLfloat g, GLfloat b) {
	glm::vec3 vec(r, g, b);
	load_vector3_f(location_fogColor, vec);
}

void SkyboxShader::connectTextureUnits() {
	load_int(location_cubeMap, 0);
	load_int(location_cubeMap2, 1);
}

void SkyboxShader::loadBlendFactor(GLfloat blend) {
	load_float(location_blendFactor, blend);
}