#pragma once

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "../shaders/ShaderProgram.h"

class TerrainShader : public ShaderProgram {
public:
	TerrainShader();

	void load_transformation_matrix(const glm::mat4& matrix) const;
	void load_projection_matrix(const glm::mat4& matrix) const;
	void load_view_matrix(const std::shared_ptr<Camera>& camera) const;
	void load_lights(const std::vector<std::shared_ptr<Light>>& lights) const;
	void load_shine_variables(float shine_damper, float reflectivity) const;
	void load_sky_color(float r, float g, float b) const;
	void load_clip_plane(const glm::vec4& plane) const;
	void load_to_shadow_map_space_matrix(const glm::mat4& to_shadow_map_space) const;

	void connect_texture_units() const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	static constexpr int MAX_LIGHTS = 4;

	// uniform variables
	int	location_transformation_matrix,
		location_projection_matrix,
		location_view_matrix,
		location_light_position[MAX_LIGHTS],
		location_light_color[MAX_LIGHTS],
		location_attenuation[MAX_LIGHTS],
		location_shine_damper,
		location_reflectivity,
		location_sky_color,
		location_background_texture,
		location_r_texture,
		location_g_texture,
		location_b_texture,
		location_blend_map,
		location_plane,
		location_to_shadow_map_space,
		location_shadow_map;
};
