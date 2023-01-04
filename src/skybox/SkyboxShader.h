#pragma once

#include "../shaders/ShaderProgram.h"
#include "../entities/Camera.h"

class SkyboxShader : public ShaderProgram {
public:
	SkyboxShader();

	void load_projection_matrix(const glm::mat4& matrix) const;
	void load_view_matrix(const std::shared_ptr<Camera>& camera);
	void load_fog_color(float r, float g, float b) const;
	void load_blend_factor(float blend) const;
	void connect_texture_units() const;

	void bind_attributes() override;
	void get_all_uniform_locations() override;

	static constexpr float ROTATE_SPEED = 0.1f;

private:
	float m_rotation = 0;

	int location_projection_matrix,
		location_view_matrix,
		location_fog_color,
		location_cube_map1,
		location_cube_map2,
		location_blend_factor;
};