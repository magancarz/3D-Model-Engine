#pragma once

#include <vector>

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "../shaders/ShaderProgram.h"

class NormalMappingShader : public ShaderProgram {
public:
	NormalMappingShader();

	void load_transformation_matrix(const glm::mat4& matrix) const;
	void load_projection_matrix(const glm::mat4& matrix) const;
	void load_view_matrix(const std::shared_ptr<Camera>& camera) const;
	void load_lights(const std::vector<std::shared_ptr<Light>>& lights, const glm::mat4& view_matrix) const;
	void load_shine_variables(float shine_damper, float reflectivity) const;
	void load_fake_lighting_variable(bool value) const;
	void load_sky_color(float r, float g, float b) const;
	void load_fog_variables(float density, float gradient) const;
	void load_number_of_rows(int value) const;
	void load_offset(float x, float y) const;
	void load_clip_plane(const glm::vec4& plane) const;

	void connect_texture_units() const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

	static constexpr int MAX_LIGHTS = 4;

private:
	static glm::vec3 get_eye_space_position(const std::shared_ptr<Light>& light, const glm::mat4& view_matrix);

	// uniform variables
	int location_transformation_matrix,
		location_projection_matrix,
		location_view_matrix,
		location_light_position_eye_space[MAX_LIGHTS],
		location_light_color[MAX_LIGHTS],
		location_attenuation[MAX_LIGHTS],
		location_shine_damper,
		location_reflectivity,
		location_use_fake_lighting,
		location_sky_color,
		location_fog_density,
		location_fog_gradient,
		location_number_of_rows,
		location_texture_offset,
		location_clip_plane,
		location_model_texture,
		location_normal_map;

};
