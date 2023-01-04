#include "NormalMappingShader.h"

#include <GL/glew.h>

#include <string>
#include <ranges>

NormalMappingShader::NormalMappingShader() :
ShaderProgram("res/shaders/normalMapVert.glsl", "res/shaders/normalMapFrag.glsl") {
	bind_attributes();
	get_all_uniform_locations();
}

void NormalMappingShader::connect_texture_units() const {
	load_int(location_model_texture, 0);
	load_int(location_normal_map, 1);
}

void NormalMappingShader::load_transformation_matrix(const glm::mat4& matrix) const {
	load_matrix(location_transformation_matrix, matrix);
}

void NormalMappingShader::load_lights(const std::vector<std::shared_ptr<Light>>& lights, const glm::mat4& view_matrix) const {
	for(const unsigned int i : std::views::iota(0, MAX_LIGHTS)) {
		if(i < lights.size()) {
			const auto& light = lights[i];
			load_vector3_f(location_light_position_eye_space[i], get_eye_space_position(light, view_matrix));
			load_vector3_f(location_light_color[i], light->get_color());
			load_vector3_f(location_attenuation[i], light->get_attenuation());
		} else {
			// If less than MAX_LIGHTS lights are in the lights vector,
			// load up empty information to the shader.
			glm::vec3 zero(0.0f, 0.0f, 0.0f);
			glm::vec3 unit(1.0f, 0.0f, 0.0f);
			load_vector3_f(location_light_position_eye_space[i], glm::vec3(0, 0, 0));
			load_vector3_f(location_light_color[i], zero);
			load_vector3_f(location_attenuation[i], unit);
		}
	}
}

void NormalMappingShader::load_projection_matrix(const glm::mat4& matrix) const {
	load_matrix(location_projection_matrix, matrix);
}

void NormalMappingShader::load_view_matrix(const std::shared_ptr<Camera>& camera) const {
	const auto view_matrix = camera->get_view();
	load_matrix(location_view_matrix, view_matrix);
}

void NormalMappingShader::load_fake_lighting_variable(const bool value) const {
	load_boolean(location_use_fake_lighting, value);
}

void NormalMappingShader::load_shine_variables(const float shine_damper, const float reflectivity) const {
	load_float(location_shine_damper, shine_damper);
	load_float(location_reflectivity, reflectivity);
}

void NormalMappingShader::load_sky_color(const float r, const float g, const float b) const {
	const glm::vec3 vec(r, g, b);
	load_vector3_f(location_sky_color, vec);
}

void NormalMappingShader::load_fog_variables(const float density, const float gradient) const {
	load_float(location_fog_density, density);
	load_float(location_fog_gradient, gradient);
} 

void NormalMappingShader::load_number_of_rows(const int value) const {
	load_float(location_number_of_rows, static_cast<float>(value));
}

void NormalMappingShader::load_offset(const float x, const float y) const {
	const glm::vec2 vec(x, y);
	load_vector2_f(location_texture_offset, vec);
}

void NormalMappingShader::load_clip_plane(const glm::vec4& plane) const {
	load_vector4_f(location_clip_plane, plane);
}

void NormalMappingShader::bind_attributes() {
	bind_attribute(0, "position");
	bind_attribute(1, "texture_coords");
	bind_attribute(2, "normal");
	bind_attribute(3, "tangent");
}

void NormalMappingShader::get_all_uniform_locations() {
	location_transformation_matrix = get_uniform_location("model");
	location_projection_matrix = get_uniform_location("proj");
	location_view_matrix = get_uniform_location("view");
	location_shine_damper = get_uniform_location("shine_damper");
	location_reflectivity = get_uniform_location("reflectivity");
	location_use_fake_lighting = get_uniform_location("use_fake_lighting");
	location_sky_color = get_uniform_location("sky_color");
	location_fog_density = get_uniform_location("fog_density");
	location_fog_gradient = get_uniform_location("fog_gradient");
	location_number_of_rows = get_uniform_location("number_of_rows");
	location_texture_offset = get_uniform_location("texture_offset");
	location_clip_plane = get_uniform_location("clip_plane");
	location_model_texture = get_uniform_location("model_texture");
	location_normal_map = get_uniform_location("normal_map");

	for(const int i : std::views::iota(0, MAX_LIGHTS)) {
		std::string str = std::to_string(i);
		location_light_position_eye_space[i] = get_uniform_location("light_position_eye_space[" + str + "]");
		location_light_color[i] = get_uniform_location("light_color[" + str + "]");
		location_attenuation[i] = get_uniform_location("attenuation[" + str + "]");
	}
}

glm::vec3 NormalMappingShader::get_eye_space_position(const std::shared_ptr<Light>& light, const glm::mat4& view_matrix) {
	glm::vec4 eye_space_pos(light->get_position(), 1.0f);
	eye_space_pos = view_matrix * eye_space_pos;

	return {eye_space_pos};
}