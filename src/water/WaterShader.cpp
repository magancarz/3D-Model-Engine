#include "WaterShader.h"

WaterShader::WaterShader()
	: ShaderProgram("res/shaders/waterVert.glsl", "res/shaders/waterFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void WaterShader::bind_attributes() {
	bind_attribute(0, "position");
}

void WaterShader::get_all_uniform_locations() {
	location_projectionMatrix = get_uniform_location("proj");
	location_viewMatrix = get_uniform_location("view");
	location_modelMatrix = get_uniform_location("model");
	location_reflectionTexture = get_uniform_location("reflectionTexture");
	location_refractionTexture = get_uniform_location("refractionTexture");
	location_dudvMap = get_uniform_location("dudvMap");
	location_normalMap = get_uniform_location("normalMap");
	location_depthMap = get_uniform_location("depthMap");
	location_moveFactor = get_uniform_location("moveFactor");
	location_lightPosition = get_uniform_location("lightPosition");
	location_lightColor = get_uniform_location("lightColor");
	location_cameraPosition = get_uniform_location("cameraPosition");
}

void WaterShader::loadProjectionMatrix(glm::mat4 projection) {
	load_matrix(location_projectionMatrix, projection);
}

void WaterShader::loadViewMatrix(Camera& camera) {
	glm::mat4 viewMatrix = camera.getView();
	load_matrix(location_viewMatrix, viewMatrix);
	load_vector3_f(location_cameraPosition, camera.getPosition());
}

void WaterShader::loadModelMatrix(glm::mat4 modelMatrix) {
	load_matrix(location_modelMatrix, modelMatrix);
}

void WaterShader::loadMoveFactor(float value) {
	load_float(location_moveFactor, value);
}

void WaterShader::loadLight(Light& sun) {
	load_vector3_f(location_lightPosition, sun.get_position());
	load_vector3_f(location_lightColor, sun.get_color());
}

void WaterShader::connectTextureUnits() {
	load_int(location_reflectionTexture, 0);
	load_int(location_refractionTexture, 1);
	load_int(location_dudvMap, 2);
	load_int(location_normalMap, 3);
	load_int(location_depthMap, 4);
}