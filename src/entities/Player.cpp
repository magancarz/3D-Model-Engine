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
	increase_rotation(0, m_current_turn_speed * DisplayManager::getFrameTimeSeconds(), 0);
	m_current_turn_speed = 0;

	float distance = m_current_speed_x * DisplayManager::getFrameTimeSeconds();
	float dx = distance * cos(glm::radians(camera_yaw - 90.f)) * cos(glm::radians(camera_pitch));
	float dz = distance * sin(glm::radians(camera_yaw - 90.f)) * cos(glm::radians(camera_pitch));
	increase_position(dx, 0, dz);

	distance = m_current_speed_z * DisplayManager::getFrameTimeSeconds();
	dx = distance * cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
	dz = distance * sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
	increase_position(dx, 0, dz);

	m_upwards_speed += GRAVITY * DisplayManager::getFrameTimeSeconds();
	increase_position(0, m_upwards_speed * DisplayManager::getFrameTimeSeconds(), 0);

	const glm::vec3 pos = get_position();
	const float terrain_height = terrain->get_height_of_terrain(pos.x, pos.z);
	if(pos.y < terrain_height) {
		m_upwards_speed = 0;
		set_position(glm::vec3(pos.x, terrain_height, pos.z));
		m_is_in_air = false;
	}
}

void Player::set_front(const glm::vec3& camera_front) { m_front = camera_front; }
void Player::set_right(const glm::vec3& camera_right) { m_right = camera_right; }

void Player::check_inputs() {
	if(Input::isKeyDown(GLFW_KEY_W)) {
		m_current_speed_x = RUN_SPEED;
	} else if(Input::isKeyDown(GLFW_KEY_S)) {
		m_current_speed_x = -RUN_SPEED;
	} else {
		m_current_speed_x = 0;
	}

	const float angle_change = DisplayManager::getMouseXOffset() * 0.1f;
	m_current_turn_speed -= angle_change;

	if(Input::isKeyDown(GLFW_KEY_A)) {
		m_current_speed_z = -RUN_SPEED;
	} else if(Input::isKeyDown(GLFW_KEY_D)) {
		m_current_speed_z = RUN_SPEED;
	} else {
		m_current_speed_z = 0;
	}

	if(Input::isKeyDown(GLFW_KEY_SPACE))
		jump();
}

void Player::jump() {
	if(!m_is_in_air) {
		m_upwards_speed = JUMP_POWER;
		m_is_in_air = true;
	}
}