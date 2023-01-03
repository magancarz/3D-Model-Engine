#include "ShadowBox.h"

#include "toolbox/DisplayManager.h"
#include "../renderEngine/MasterRenderer.h"

ShadowBox::ShadowBox(glm::mat4* light_view_matrix, std::shared_ptr<Camera> camera) :
m_light_view_matrix(light_view_matrix), m_camera(std::move(camera)) {

	calculate_widths_and_heights();
}

void ShadowBox::update() {
	const glm::mat4 rotation = calculate_camera_rotation_matrix();
	const glm::vec3 forward_vector = rotation * FORWARD;

	auto to_far = glm::vec3(forward_vector);
	to_far *= SHADOW_DISTANCE;
	auto to_near = glm::vec3(forward_vector);
	to_near *= MasterRenderer::NEAR_PLANE;
	const glm::vec3 center_near = to_near + m_camera->get_position();
	const glm::vec3 center_far = to_far + m_camera->get_position();

	const auto points = calculate_frustum_vertices(rotation, forward_vector, center_near, center_far);

	bool first = true;
	for(const auto point : points) {
		if(first) {
			m_min_x = point.x;
			m_max_x = point.x;
			m_min_y = point.y;
			m_max_y = point.y;
			m_min_z = point.z;
			m_max_z = point.z;
			first = false;
			continue;
		}

		if(point.x > m_max_x) {
			m_max_x = point.x;
		} else if(point.x < m_min_x) {
			m_min_x = point.x;
		}

		if(point.y > m_max_y) {
			m_max_y = point.y;
		} else if(point.y < m_min_y) {
			m_min_y = point.y;
		}

		if(point.z > m_max_z) {
			m_max_z = point.z;
		} else if(point.z < m_min_z) {
			m_min_z = point.z;
		}
	}
	m_max_z += OFFSET;
}

glm::vec3 ShadowBox::get_center() const{
	const float x = (m_min_x + m_max_x) / 2.f;
	const float y = (m_min_y + m_max_y) / 2.f;
	const float z = (m_min_z + m_max_z) / 2.f;
	const auto cen = glm::vec4(x, y, z, 1.f);
	const glm::mat4 inverted_light = glm::inverse(*m_light_view_matrix);
	return { inverted_light * cen };
}

void ShadowBox::set_light_view_matrix(glm::mat4* light_view_matrix) { m_light_view_matrix = light_view_matrix; }

float ShadowBox::get_width() const {
	return m_max_x - m_min_x;
}

float ShadowBox::get_height() const {
	return m_max_y - m_min_y;
}

float ShadowBox::get_length() const {
	return m_max_z - m_min_z;
}

std::vector<glm::vec4> ShadowBox::calculate_frustum_vertices(
		const glm::mat4& rotation,
		const glm::vec3& forward_vector,
		const glm::vec3& center_near,
		const glm::vec3& center_far) const {

	glm::vec3 up_vector = rotation * UP;
	glm::vec3 right_vector = glm::cross(forward_vector, up_vector);
	glm::vec3 down_vector = glm::vec3(-up_vector.x, -up_vector.y, -up_vector.z);
	glm::vec3 left_vector = glm::vec3(-right_vector.x, -right_vector.y, -right_vector.z);
	glm::vec3 far_top = center_far + glm::vec3(up_vector.x * m_far_height, up_vector.y * m_far_height, up_vector.z * m_far_height);
	glm::vec3 far_bottom = center_far + glm::vec3(down_vector.x * m_far_height, down_vector.y * m_far_height, down_vector.z * m_far_height);
	glm::vec3 near_top = center_near + glm::vec3(up_vector.x * m_near_height, up_vector.y * m_near_height, up_vector.z * m_near_height);
	glm::vec3 near_bottom = center_near + glm::vec3(down_vector.x * m_near_height, down_vector.y * m_near_height, down_vector.z * m_near_height);

	std::vector<glm::vec4> points;
	points.push_back(calculate_light_space_frustum_corner(far_top, right_vector, m_far_width));
	points.push_back(calculate_light_space_frustum_corner(far_top, left_vector, m_far_width));
	points.push_back(calculate_light_space_frustum_corner(far_bottom, right_vector, m_far_width));
	points.push_back(calculate_light_space_frustum_corner(far_bottom, left_vector, m_far_width));
	points.push_back(calculate_light_space_frustum_corner(near_top, right_vector, m_near_width));
	points.push_back(calculate_light_space_frustum_corner(near_top, left_vector, m_near_width));
	points.push_back(calculate_light_space_frustum_corner(near_bottom, right_vector, m_near_width));
	points.push_back(calculate_light_space_frustum_corner(near_bottom, left_vector, m_near_width));

	return points;
}

glm::vec4 ShadowBox::calculate_light_space_frustum_corner(const glm::vec3& start_point, const glm::vec3& direction, const float width) const {
	const auto point = start_point + glm::vec3(direction.x * width, direction.y * width, direction.z * width);
	const auto point4_f = glm::vec4((*m_light_view_matrix) * glm::vec4(point.x, point.y, point.z, 1.f));
	return point4_f;
}

glm::mat4 ShadowBox::calculate_camera_rotation_matrix() const {
	glm::mat4 rotation;
	rotation = glm::rotate(rotation, glm::radians(-m_camera->get_yaw()), glm::vec3(0, 1, 0));
	rotation = glm::rotate(rotation, glm::radians(-m_camera->get_pitch()), glm::vec3(1, 0, 0));
	return rotation;
}

void ShadowBox::calculate_widths_and_heights() {
	m_far_width = SHADOW_DISTANCE * glm::tan(glm::radians(MasterRenderer::FOV));
	m_near_width = MasterRenderer::NEAR_PLANE * glm::tan(glm::radians(MasterRenderer::FOV));
	m_far_height = m_far_width / get_aspect_ratio();
	m_near_height = m_near_width / get_aspect_ratio();
}

float ShadowBox::get_aspect_ratio() {
	return static_cast<float>(DisplayManager::WINDOW_WIDTH) / static_cast<float>(DisplayManager::WINDOW_HEIGHT);
}