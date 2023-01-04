#include "WaterShader.h"

WaterShader::WaterShader() :
ShaderProgram("res/shaders/waterVert.glsl", "res/shaders/waterFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void WaterShader::load_projection_matrix(const glm::mat4& projection) const {
	load_matrix(location_projection_matrix, projection);
}

void WaterShader::load_view_matrix(const std::shared_ptr<Camera>& camera) const {
	const auto view_matrix = camera->get_view();
	load_matrix(location_view_matrix, view_matrix);
	load_vector3_f(location_camera_position, camera->get_position());
}

void WaterShader::load_model_matrix(const glm::mat4& model_matrix) const {
	load_matrix(location_model_matrix, model_matrix);
}

void WaterShader::load_move_factor(const float value) const {
	load_float(location_move_factor, value);
}

void WaterShader::load_light(const std::shared_ptr<Light>& sun) const {
	load_vector3_f(location_light_position, sun->get_position());
	load_vector3_f(location_light_color, sun->get_color());
}

void WaterShader::connect_texture_units() const {
	load_int(location_reflection_texture, 0);
	load_int(location_refraction_texture, 1);
	load_int(location_dudv_map, 2);
	load_int(location_normal_map, 3);
	load_int(location_depth_map, 4);
}

void WaterShader::bind_attributes() {
	bind_attribute(0, "position");
}

void WaterShader::get_all_uniform_locations() {
	location_projection_matrix = get_uniform_location("proj");
	location_view_matrix = get_uniform_location("view");
	location_model_matrix = get_uniform_location("model");
	location_reflection_texture = get_uniform_location("reflection_texture");
	location_refraction_texture = get_uniform_location("refraction_texture");
	location_dudv_map = get_uniform_location("dudv_map");
	location_normal_map = get_uniform_location("normal_map");
	location_depth_map = get_uniform_location("depth_map");
	location_move_factor = get_uniform_location("move_factor");
	location_light_position = get_uniform_location("light_position");
	location_light_color = get_uniform_location("light_color");
	location_camera_position = get_uniform_location("camera_position");
}