#include "StaticShader.h"

#include <ranges>

StaticShader::StaticShader() :
ShaderProgram("res/shaders/vert.glsl", "res/shaders/frag.glsl" ),
location_transformation_matrix(0) {
	bind_attributes();
	get_all_uniform_locations();
}

void StaticShader::load_transformation_matrix(const glm::mat4& matrix) const {
	load_matrix(location_transformation_matrix, matrix);
}

void StaticShader::load_projection_matrix(const glm::mat4& matrix) const {
	load_matrix(location_projection_matrix, matrix);
}

void StaticShader::load_view_matrix(const std::shared_ptr<Camera>& camera) const {
	const auto view = camera->get_view();
	load_matrix(location_view_matrix, view);
}

void StaticShader::load_lights(const std::vector<std::shared_ptr<Light>>& lights) const {
	for(const unsigned int i : std::views::iota(0, MAX_LIGHTS)) {
		if(i < lights.size()) {
			load_vector3_f(location_light_position[i], lights[i]->get_position());
			load_vector3_f(location_light_color[i], lights[i]->get_color());
			load_vector3_f(location_attenuation[i], lights[i]->get_attenuation());
		} else {
			load_vector3_f(location_light_position[i], glm::vec3(0));
			load_vector3_f(location_light_color[i], glm::vec3(0));
			load_vector3_f(location_attenuation[i], glm::vec3(1, 0, 0));
		}
	}
}

void StaticShader::load_shine_variables(const float shine_damper, const float reflectivity) const {
	load_float(location_shine_damper, shine_damper);
	load_float(location_reflectivity, reflectivity);
}

void StaticShader::load_fake_lighting_variable(const bool value) const {
	load_boolean(location_use_fake_lighting, value);
}

void StaticShader::load_sky_color(const float r, const float g, const float b) const {
	load_vector3_f(location_sky_color, glm::vec3(r, g, b));
}

void StaticShader::load_number_of_rows(const float value) const {
	load_float(location_number_of_rows, value);
}

void StaticShader::load_offset(const glm::vec2& offset) const {
	load_vector2_f(location_offset, offset);
}

void StaticShader::connect_texture_units() const {
	load_int(location_model_texture, 0);
	load_int(location_specular_map, 1);
}

void StaticShader::load_use_specular_map(const bool use_map) const {
	load_boolean(location_uses_specular_map, use_map);
}

void StaticShader::load_clip_plane(const glm::vec4& plane) const {
	load_vector4_f(location_plane, plane);
}

void StaticShader::bind_attributes() {
	bind_attribute(0, "position");
	bind_attribute(1, "textureCoords");
	bind_attribute(2, "normal");
}

void StaticShader::get_all_uniform_locations() {
	//MVP calculation uniforms
	location_transformation_matrix = get_uniform_location("model");
	location_projection_matrix = get_uniform_location("proj");
	location_view_matrix = get_uniform_location("view");

	//Light calculation uniforms
	for(const int i : std::views::iota(0, MAX_LIGHTS)) {
		location_light_position[i] = get_uniform_location("lightPosition[" + std::to_string(i) + "]");
		location_light_color[i] = get_uniform_location("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = get_uniform_location("attenuation[" + std::to_string(i) + "]");
	}
	location_shine_damper = get_uniform_location("shineDamper");
	location_reflectivity = get_uniform_location("reflectivity");
	location_use_fake_lighting = get_uniform_location("useFakeLighting");

	//Fog calculation uniform
	location_sky_color = get_uniform_location("skyColor");

	//Texture atlases uniforms
	location_number_of_rows = get_uniform_location("numberOfRows");
	location_offset = get_uniform_location("offset");

	//Clip plane
	location_plane = get_uniform_location("plane");

	//Specular map uniforms
	location_specular_map = get_uniform_location("specularSampler");
	location_model_texture = get_uniform_location("textureSampler");
	location_uses_specular_map = get_uniform_location("usesSpecularMap");
}