#include "Player.h"

Player::Player(TexturedModel& model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale)
	: Entity(model, position, rotX, rotY, rotZ, scale) {

}

void Player::move(Terrain& terrain) {
	checkInputs();
	increaseRotation(0, m_currentTurnSpeed * display.getFrameTimeSeconds(), 0);
	float distance = m_currentSpeed * display.getFrameTimeSeconds();
	float dx = distance * glm::sin(glm::radians(getRotY()));
	float dz = distance * glm::cos(glm::radians(getRotY()));
	increasePosition(dx, 0, dz);
	m_upwardsSpeed += GRAVITY * display.getFrameTimeSeconds();
	increasePosition(0, m_upwardsSpeed * display.getFrameTimeSeconds(), 0);
	glm::vec3 pos = getPosition();
	float terrainHeight = terrain.getHeightOfTerrain(pos.x, pos.z);
	if(pos.y < terrainHeight) {
		m_upwardsSpeed = 0;
		setPosition(glm::vec3(pos.x, terrainHeight, pos.z));
		isInAir = false;
	}
}

void Player::checkInputs() {
	if(inputManager.isKeyDown(GLFW_KEY_W)) {
		m_currentSpeed = RUN_SPEED;
	} else if(inputManager.isKeyDown(GLFW_KEY_S)) {
		m_currentSpeed = -RUN_SPEED;
	} else {
		m_currentSpeed = 0;
	}

	if(inputManager.isKeyDown(GLFW_KEY_A)) {
		m_currentTurnSpeed = TURN_SPEED;
	} else if(inputManager.isKeyDown(GLFW_KEY_D)) {
		m_currentTurnSpeed = -TURN_SPEED;
	} else {
		m_currentTurnSpeed = 0;
	}

	if(inputManager.isKeyDown(GLFW_KEY_SPACE))
		jump();
}

void Player::jump() {
	if(!isInAir) {
		m_upwardsSpeed = JUMP_POWER;
		isInAir = true;
	}
}