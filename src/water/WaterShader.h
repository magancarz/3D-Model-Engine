#pragma once

#include "../entities/Camera.h"
#include "../entities/Light.h"
#include "shaders/ShaderProgram.h"

class WaterShader : public ShaderProgram {
public:
	WaterShader();

	void load_projection_matrix(const glm::mat4& projection) const;
	void load_view_matrix(const std::shared_ptr<Camera>& camera) const;
	void load_model_matrix(const glm::mat4& model_matrix) const;
	void load_move_factor(float value) const;
	void load_light(const std::shared_ptr<Light>& sun) const;

	void connect_texture_units() const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

private:
	// uniform variables
	int location_model_matrix,
		location_view_matrix,
		location_projection_matrix,
		location_reflection_texture,
		location_refraction_texture,
		location_dudv_map,
		location_normal_map,
		location_depth_map,
		location_move_factor,
		location_light_position,
		location_light_color,
		location_camera_position;

};
