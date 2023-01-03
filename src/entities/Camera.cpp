#include "Camera.h"

#include "toolbox/DisplayManager.h"

Camera::Camera(std::shared_ptr<Player> player, const glm::vec3& position) :
m_player(std::move(player)), m_position(position) {

	m_movement_speed = 0.3f;
	m_sensitivity = 0.35f;

	m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(0.0f);
	m_up = m_world_up;

	m_pitch = 10.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;
}

void Camera::invert_pitch() {
	m_pitch = -m_pitch;
}

glm::mat4 Camera::get_view() const {
	auto view_matrix = glm::mat4(1.0f);

	view_matrix = glm::rotate(view_matrix, glm::radians(get_pitch()), glm::vec3(1, 0, 0));
	view_matrix = glm::rotate(view_matrix, glm::radians(get_yaw()), glm::vec3(0, 1, 0));

	const auto camera_pos = get_position();
	const auto negative_camera_pos = glm::vec3(-camera_pos[0], -camera_pos[1], -camera_pos[2]);

	view_matrix = glm::translate(view_matrix, negative_camera_pos);

	return view_matrix;
}

void Camera::rotate() {
	const float yaw = DisplayManager::get_mouse_x_offset();
	const float pitch = DisplayManager::get_mouse_y_offset();
	m_pitch += yaw * m_sensitivity;
	m_yaw += pitch * m_sensitivity;

	if(m_pitch > 80.f)
		m_pitch = 80.f;
	else if(m_pitch < -80.f)
		m_pitch = -80.f;

	if(m_yaw > 360.f || m_yaw < -360.f)
		m_yaw = 0.f;
}

void Camera::move() {
	update_camera_vectors();

	//calculate_zoom();
	calculate_pitch();
	calculate_angle_around_the_player();
	//float horizontal_distance = calculate_horizontal_distance();
	//float vertical_distance = calculate_vertical_distance();

	m_yaw = 180.f - (m_player->get_rot_y() + m_angle_around_the_player);
	m_yaw = fmod(m_yaw, 360.f);

	m_position.x = m_player->get_position().x;
	m_position.y = m_player->get_position().y + CAMERA_HEIGHT;
	m_position.z = m_player->get_position().z;
}

void Camera::calculate_zoom() {
	const float zoom_level = DisplayManager::get_d_wheel() * 1.0f;
	m_distance_from_the_player -= zoom_level;
}

void Camera::calculate_pitch() {
	//3rd person camera code
	/*if(inputManager.isRightMouseButtonDown()) {
		float pitchChange = display.getMouseYOffset() * 0.1f;
		m_pitch -= pitchChange;
	}*/

	const float pitch_change = DisplayManager::get_mouse_y_offset() * 0.1f;
	m_pitch += pitch_change;
}

void Camera::calculate_angle_around_the_player() {
	//3rd person camera code
	/*if(inputManager.isLeftMouseButtonDown()) {
		float angleChange = display.getMouseXOffset() * 0.1f;
		m_angle_around_the_player -= angleChange;
	}*/
	const float angle_change = DisplayManager::get_mouse_x_offset() * 0.1f;
	m_angle_around_the_player -= angle_change;
}

float Camera::calculate_horizontal_distance() const {
	return m_distance_from_the_player * glm::cos(glm::radians(m_pitch));
}

float Camera::calculate_vertical_distance() const {
	return m_distance_from_the_player * glm::sin(glm::radians(m_pitch));
}

void Camera::calculate_camera_position(const float horizontal_distance, const float vertical_distance) {
	const float theta = m_player->get_rot_y() + m_angle_around_the_player;
	const float offset_x = horizontal_distance * glm::sin(glm::radians(theta));
	const float offset_z = horizontal_distance * glm::cos(glm::radians(theta));
	const auto player_position = m_player->get_position();
	m_position.x = player_position.x - offset_x;
	m_position.y = player_position.y + vertical_distance;
	m_position.z = player_position.z - offset_z;
}

void Camera::update_camera_vectors() {
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(m_front);
	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	m_player->set_front(m_front);
	m_player->set_right(m_right);
}

void Camera::set_position(const glm::vec3& position) { m_position = position; }
void Camera::set_pitch(const float pitch) { m_pitch = pitch; }
void Camera::set_yaw(const float yaw) { m_yaw = yaw; }
void Camera::set_roll(const float roll) { m_roll = roll; }

void Camera::increase_pitch(const float pitch) { m_pitch += pitch * m_sensitivity; }
void Camera::increase_yaw(const float yaw) { m_yaw += yaw * m_sensitivity; }
void Camera::increase_roll(const float roll) { m_roll += roll * m_sensitivity; }

glm::vec3 Camera::get_position() const { return m_position; }
float Camera::get_pitch() const { return m_pitch; }
float Camera::get_yaw() const { return m_yaw; }
float Camera::get_roll() const { return m_roll; }
float Camera::get_sensitivity() const { return m_sensitivity; }
glm::vec3 Camera::get_camera_front() const { return m_front; }
glm::vec3 Camera::get_camera_right() const { return m_right; }