#pragma once

#include "../entities/Camera.h"

class ShadowBox {
public:
	/**
	Creates a new shadow box and calculates some initial values relating to
	the camera's view frustum, namely the width and height of the near plane
	and (possibly adjusted) far plane.
	*/
	ShadowBox(glm::mat4* light_view_matrix, std::shared_ptr<Camera> camera);

	/**
	Updates the bounds of the shadow box based on the light direction and the
	camera's view frustum, to make sure that the box covers the smallest area
	possible while still ensuring that everything inside the camera's view
	(within a ceratin range) will cast shadows.
	*/
	void update();

	/**
	Calculates the center of the "view cuboid" in light space first, and then
	converts this to world space using the inverse light's view matrix.
	*/
	glm::vec3 get_center() const;

	void set_light_view_matrix(glm::mat4* light_view_matrix);

	float get_width() const;
	float get_height() const;
	float get_length() const;

private:
	/**
	Calculates the position of the vertex at each corner of the view frustum
	in light space (8 vertices in total, so this returns 8 QUAD_POSITIONS).
	*/
	std::vector<glm::vec4> calculate_frustum_vertices(
		const glm::mat4& rotation,
		const glm::vec3& forward_vector,
		const glm::vec3& center_near,
		const glm::vec3& center_far) const;

	/**
	Calculates one of the corner vertices of the view frustum in world space
	and converts it to light space.
	*/
	glm::vec4 calculate_light_space_frustum_corner(const glm::vec3& start_point, const glm::vec3& direction, float width) const;

	/**
	The m_rotation of the camera represented as a matrix.
	*/
	glm::mat4 calculate_camera_rotation_matrix() const;

	/**
	Calculates the width and height of the near and far planes of the
	camera's view frustum. However, this doesn't have to use the "actual" far
	plane of the view frustum. It can use a shortened view frustum if desired
	by bringing the far-plane closer, which would increase shadow resolution
	but means that distant objects wouldn't cast shadows.
	*/
	void calculate_widths_and_heights();

	/**
	The aspect ratio of the display (width:height ratio).
	*/
	static float get_aspect_ratio();

	const float OFFSET          = 10.0f,
				SHADOW_DISTANCE = 100.0f;

	const glm::vec4 UP      = glm::vec4(0, 1, 0, 0),
					FORWARD = glm::vec4(0, 0, -1, 0);

	float m_min_x = 0, m_max_x = 0,
		  m_min_y = 0, m_max_y = 0,
		  m_min_z = 0, m_max_z = 0;

	glm::mat4* m_light_view_matrix;
	std::shared_ptr<Camera> m_camera;

	float m_far_height, m_far_width, m_near_height, m_near_width;
};