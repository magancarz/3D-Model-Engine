#include "Player.h"

#include <iostream>
#include <GL/glew.h>

#include "toolbox/DisplayManager.h"
#include "toolbox/Input.h"

Player::Player(
		std::shared_ptr<TexturedModel> model,
		const glm::vec3& position,
		const float rot_x, const float rot_y, const float rot_z, const float scale) :
Entity(std::move(model), position, rot_x, rot_y, rot_z, scale) {

}

void Player::move(const std::shared_ptr<Terrain>& terrain, const float camera_yaw, const float camera_pitch) {
	check_inputs();
	increase_rotation(0, m_current_turn_speed * static_cast<float>(DisplayManager::get_frame_time_seconds()), 0);
	m_current_turn_speed = 0;

	float distance = m_current_speed_x * static_cast<float>(DisplayManager::get_frame_time_seconds());
	float dx = distance * cos(glm::radians(camera_yaw - 90.f));
	float dz = distance * sin(glm::radians(camera_yaw - 90.f));
	increase_position(dx, 0, dz);

	distance = m_current_speed_z * static_cast<float>(DisplayManager::get_frame_time_seconds());
	dx = distance * cos(glm::radians(camera_yaw));
	dz = distance * sin(glm::radians(camera_yaw));
	increase_position(dx, 0, dz);

	m_upwards_speed += GRAVITY * static_cast<float>(DisplayManager::get_frame_time_seconds());
	increase_position(0, m_upwards_speed * static_cast<float>(DisplayManager::get_frame_time_seconds()), 0);

	const glm::vec3 pos = get_position();
	const float terrain_height = terrain->get_height_of_terrain(pos.x, pos.z);
	if(pos.y < terrain_height) {
		m_upwards_speed = 0;
		set_position(glm::vec3(pos.x, terrain_height, pos.z));
		m_is_in_air = false;
	}
}

void Player::set_front(const glm::vec3& camera_front) {
	m_front = camera_front;
}

void Player::set_right(const glm::vec3& camera_right) {
	m_right = camera_right;
}

void Player::check_inputs() {
	if(Input::is_key_down(GLFW_KEY_W)) {
		m_current_speed_x = RUN_SPEED;
	} else if(Input::is_key_down(GLFW_KEY_S)) {
		m_current_speed_x = -RUN_SPEED;
	} else {
		m_current_speed_x = 0;
	}

	const auto angle_change = static_cast<float>(DisplayManager::get_mouse_x_offset() * 0.1);
	m_current_turn_speed -= angle_change;

	if(Input::is_key_down(GLFW_KEY_A)) {
		m_current_speed_z = -RUN_SPEED;
	} else if(Input::is_key_down(GLFW_KEY_D)) {
		m_current_speed_z = RUN_SPEED;
	} else {
		m_current_speed_z = 0;
	}

	if(Input::is_key_down(GLFW_KEY_SPACE))
		jump();
}

void Player::jump() {
	if(!m_is_in_air) {
		m_upwards_speed = JUMP_POWER;
		m_is_in_air = true;
	}
}