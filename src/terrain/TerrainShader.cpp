#include "TerrainShader.h"

TerrainShader::TerrainShader() :
ShaderProgram("res/shaders/terrainVert.glsl", "res/shaders/terrainFrag.glsl") {

	bind_attributes();
	get_all_uniform_locations();
}

void TerrainShader::load_transformation_matrix(const glm::mat4& matrix) const {
	load_matrix(location_transformation_matrix, matrix);
}

void TerrainShader::load_projection_matrix(const glm::mat4& matrix) const {
	load_matrix(location_projection_matrix, matrix);
}

void TerrainShader::load_view_matrix(const std::shared_ptr<Camera>& camera) const {
	const auto view = camera->get_view();
	load_matrix(location_view_matrix, view);
}

void TerrainShader::load_lights(const std::vector<std::shared_ptr<Light>>& lights) const {
	for(size_t i = 0; i < MAX_LIGHTS; i++) {
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

void TerrainShader::load_shine_variables(const float shine_damper, const float reflectivity) const {
	load_float(location_shine_damper, shine_damper);
	load_float(location_reflectivity, reflectivity);
}

void TerrainShader::load_sky_color(const float r, const float g, const float b) const {
	load_vector3_f(location_sky_color, glm::vec3(r, g, b));
}

void TerrainShader::load_clip_plane(const glm::vec4& plane) const {
	load_vector4_f(location_plane, plane);
}

void TerrainShader::load_to_shadow_map_space_matrix(const glm::mat4& to_shadow_map_space) const {
	load_matrix(location_to_shadow_map_space, to_shadow_map_space);
}

void TerrainShader::bind_attributes() {
	bind_attribute(0, "position");
	bind_attribute(1, "textureCoords");
	bind_attribute(2, "normal");
}

void TerrainShader::get_all_uniform_locations() {
	//MVP calculation uniforms
	location_transformation_matrix = get_uniform_location("model");
	location_projection_matrix = get_uniform_location("proj");
	location_view_matrix = get_uniform_location("view");

	//Light calculation uniforms
	for(int i = 0; i < MAX_LIGHTS; i++) {
		location_light_position[i] = get_uniform_location("lightPosition[" + std::to_string(i) + "]");
		location_light_color[i] = get_uniform_location("lightColor[" + std::to_string(i) + "]");
		location_attenuation[i] = get_uniform_location("attenuation[" + std::to_string(i) + "]");
	}
	location_shine_damper = get_uniform_location("shineDamper");
	location_reflectivity = get_uniform_location("reflectivity");

	//Fog calculation uniforms
	location_sky_color = get_uniform_location("skyColor");

	//Texture samplers
	location_background_texture = get_uniform_location("backgroundTexture");
	location_r_texture = get_uniform_location("rTexture");
	location_g_texture = get_uniform_location("gTexture");
	location_b_texture = get_uniform_location("bTexture");
	location_blend_map = get_uniform_location("blendMap");

	//Clip plane
	location_plane = get_uniform_location("plane");

	//Shadows
	location_to_shadow_map_space = get_uniform_location("toShadowMapSpace");
	location_shadow_map = get_uniform_location("shadowMap");
}

void TerrainShader::connect_texture_units() const {
	load_int(location_background_texture, 0);
	load_int(location_r_texture, 1);
	load_int(location_g_texture, 2);
	load_int(location_b_texture, 3);
	load_int(location_blend_map, 4);
	load_int(location_shadow_map, 5);
}