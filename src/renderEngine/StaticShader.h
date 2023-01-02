#pragma once

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "shaders/ShaderProgram.h"

class StaticShader : public ShaderProgram {
public:
	StaticShader();

	void load_transformation_matrix(const glm::mat4& matrix) const;
	void load_projection_matrix(const glm::mat4& matrix) const;
	void load_view_matrix(const std::shared_ptr<Camera>& camera) const;
	void load_lights(const std::vector<std::shared_ptr<Light>>& lights) const;
	void load_shine_variables(float shine_damper, float reflectivity) const;
	void load_fake_lighting_variable(bool value) const;
	void load_sky_color(float r, float g, float b) const;
	void load_number_of_rows(float value) const;
	void load_offset(const glm::vec2& offset) const;
	void load_clip_plane(const glm::vec4& plane) const;
	void connect_texture_units() const;
	void load_use_specular_map(bool use_map) const;
	
	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	static constexpr int MAX_LIGHTS = 4;

	// uniform locations
	int location_transformation_matrix,
		location_projection_matrix,
		location_view_matrix,
		location_light_position[MAX_LIGHTS],
		location_light_color[MAX_LIGHTS],
		location_attenuation[MAX_LIGHTS],
		location_shine_damper,
		location_reflectivity,
		location_use_fake_lighting,
		location_sky_color,
		location_number_of_rows,
		location_offset,
		location_plane,
		location_specular_map,
		location_model_texture,
		location_uses_specular_map;
};
