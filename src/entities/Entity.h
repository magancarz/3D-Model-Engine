#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../models/TexturedModel.h"

const float GRAVITY = -50.0f;

class Entity {
public:
	Entity(std::shared_ptr<TexturedModel> model, const glm::vec3& position, float rx, float ry, float rz, float scale);

	Entity(std::shared_ptr<TexturedModel> model, int texture_index, const glm::vec3& position, float rx, float ry, float rz, float scale);
	
	void increase_position(float dx, float dy, float dz);
	void increase_rotation(float rx, float ry, float rz);

	float get_texture_x_offset();
	float get_texture_y_offset();

	std::shared_ptr<TexturedModel> get_textured_model();
	glm::vec3 get_position() const;
	float get_rot_x() const;
	float get_rot_y() const;
	float get_rot_z() const;
	float get_scale() const;

	void set_textured_model(const std::shared_ptr<TexturedModel>& model);
	void set_position(const glm::vec3& position);
	void set_rot_x(float rx);
	void set_rot_y(float ry);
	void set_rot_z(float rz);
	void set_scale(float scale);
	
	const float TERRAIN_HEIGHT = 0;
	
	bool m_is_in_air = false;

private:
	std::shared_ptr<TexturedModel> m_model;

	glm::vec3 m_position;
	float m_rx, m_ry, m_rz;
	float m_scale;

	int m_texture_index = 0;
};