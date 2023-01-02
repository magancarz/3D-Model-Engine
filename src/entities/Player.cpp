#include "Player.h"

#include <GL/glew.h>

#include "toolbox/DisplayManager.h"
#include "toolbox/Input.h"

Player::Player(std::shared_ptr<TexturedModel> model, const glm::vec3& position, float rotX, float rotY, float rotZ, float scale)
	: Entity(model, position, rotX, rotY, rotZ, scale) {

}

void Player::move(Terrain& terrain, float cameraYaw, float cameraPitch) {
	checkInputs();
	increase_rotation(0, m_currentTurnSpeed * DisplayManager::getFrameTimeSeconds(), 0);
	m_currentTurnSpeed = 0;

	float distance = m_currentSpeedX * DisplayManager::getFrameTimeSeconds();
	float dx = distance * cos(glm::radians(cameraYaw - 90.f)) * cos(glm::radians(cameraPitch));
	float dz = distance * sin(glm::radians(cameraYaw - 90.f)) * cos(glm::radians(cameraPitch));
	increase_position(dx, 0, dz);

	distance = m_currentSpeedZ * DisplayManager::getFrameTimeSeconds();
	dx = distance * cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	dz = distance * sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	increase_position(dx, 0, dz);

	m_upwardsSpeed += GRAVITY * DisplayManager::getFrameTimeSeconds();
	increase_position(0, m_upwardsSpeed * DisplayManager::getFrameTimeSeconds(), 0);
	glm::vec3 pos = get_position();
	float terrainHeight = terrain.get_height_of_terrain(pos.x, pos.z);
	if(pos.y < terrainHeight) {
		m_upwardsSpeed = 0;
		set_position(glm::vec3(pos.x, terrainHeight, pos.z));
		m_is_in_air = false;
	}
}

void Player::checkInputs() {
	if(Input::isKeyDown(GLFW_KEY_W)) {
		m_currentSpeedX = RUN_SPEED;
	} else if(Input::isKeyDown(GLFW_KEY_S)) {
		m_currentSpeedX = -RUN_SPEED;
	} else {
		m_currentSpeedX = 0;
	}

	float angleChange = DisplayManager::getMouseXOffset() * 0.1f;
	m_currentTurnSpeed -= angleChange;

	if(Input::isKeyDown(GLFW_KEY_A)) {
		m_currentSpeedZ = -RUN_SPEED;
	} else if(Input::isKeyDown(GLFW_KEY_D)) {
		m_currentSpeedZ = RUN_SPEED;
	} else {
		m_currentSpeedZ = 0;
	}

	if(Input::isKeyDown(GLFW_KEY_SPACE))
		jump();
}

void Player::jump() {
	if(!m_is_in_air) {
		m_upwardsSpeed = JUMP_POWER;
		m_is_in_air = true;
	}
}