#include "SkyboxShader.h"

#include "toolbox/DisplayManager.h"

SkyboxShader::SkyboxShader() :
ShaderProgram("res/shaders/skyboxVertShader.glsl", "res/shaders/skyboxFragShader.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}


void SkyboxShader::load_projection_matrix(const glm::mat4& matrix) const {
	load_matrix(location_projection_matrix, matrix);
}

void SkyboxShader::load_view_matrix(const std::shared_ptr<Camera>& camera) {
	glm::mat4 view = camera->get_view();
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	m_rotation += ROTATE_SPEED * DisplayManager::get_frame_time_seconds();
	view = glm::rotate(view, glm::radians(m_rotation), glm::vec3(0, 1, 0));

	load_matrix(location_view_matrix, view);
}

void SkyboxShader::load_fog_color(const float r, const float g, const float b) const {
	const glm::vec3 vec(r, g, b);
	load_vector3_f(location_fog_color, vec);
}

void SkyboxShader::load_blend_factor(const float blend) const {
	load_float(location_blend_factor, blend);
}

void SkyboxShader::connect_texture_units() const {
	load_int(location_cube_map1, 0);
	load_int(location_cube_map2, 1);
}

void SkyboxShader::bind_attributes() {
	bind_attribute(0, "position");
}

void SkyboxShader::get_all_uniform_locations() {
	location_projection_matrix = get_uniform_location("projectionMatrix");
	location_view_matrix = get_uniform_location("viewMatrix");
	location_fog_color = get_uniform_location("fogColor");
	location_cube_map1 = get_uniform_location("cubeMap");
	location_cube_map2 = get_uniform_location("cubeMap2");
	location_blend_factor = get_uniform_location("blendFactor");
}