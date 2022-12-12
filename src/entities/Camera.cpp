#include "Camera.h"

Camera::Camera(Player& player, glm::vec3 position)
	: m_player(player), m_position(position) {
	m_movementSpeed = 0.3f;
	m_sensitivity = 0.35f;

	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(0.0f);
	m_up = m_worldUp;

	m_pitch = 10.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;

	//updateCameraVectors();
}

void Camera::invertPitch() {
	m_pitch = -m_pitch;
}

glm::mat4 Camera::getView() {
	glm::mat4 viewMatrix = glm::mat4(1.0f); // identity matrix

	viewMatrix = glm::rotate(viewMatrix, glm::radians(getPitch()), glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(getYaw()), glm::vec3(0, 1, 0));

	glm::vec3 cameraPos = getPosition();
	glm::vec3 negativeCameraPos = glm::vec3(-cameraPos[0], -cameraPos[1], -cameraPos[2]);

	viewMatrix = glm::translate(viewMatrix, negativeCameraPos);

	return viewMatrix;
}

void Camera::rotate() {
	float yaw = display.getMouseXOffset();
	float pitch = display.getMouseYOffset();
	m_pitch += static_cast<float>(yaw) * m_sensitivity;
	m_yaw += static_cast<float>(pitch) * m_sensitivity;

	if(this->m_pitch > 80.f)
		this->m_pitch = 80.f;
	else if(this->m_pitch < -80.f)
		this->m_pitch = -80.f;

	if(this->m_yaw > 360.f || this->m_yaw < -360.f)
		this->m_yaw = 0.f;
}

void Camera::move() {
	//updateCameraVectors();

	calculateZoom();
	calculatePitch();
	calculateAngleAroundThePlayer();
	float horizontalDistance = calculateHorizontalDistance();
	float verticalDistance = calculateVerticalDistance();
	calculateCameraPosition(horizontalDistance, verticalDistance);

	m_yaw = 180.f - (m_player.getRotY() + m_angleAroundThePlayer);
	m_yaw = (float) fmod(m_yaw, 360.f);

	/*
	if(inputManager.isKeyDown(GLFW_KEY_W))
		m_position += m_front * m_movementSpeed;
	else if(inputManager.isKeyDown(GLFW_KEY_S))
		m_position -= m_front * m_movementSpeed;
	if(inputManager.isKeyDown(GLFW_KEY_A))
		m_position -= m_right * m_movementSpeed;
	else if(inputManager.isKeyDown(GLFW_KEY_D))
		m_position += m_right * m_movementSpeed;
	*/
}

void Camera::calculateZoom() {
	float zoomLevel = display.getDWheel() * 1.0f;
	m_distanceFromThePlayer -= zoomLevel;
}

void Camera::calculatePitch() {
	if(inputManager.isRightMouseButtonDown()) {
		float pitchChange = display.getMouseYOffset() * 0.1f;
		m_pitch -= pitchChange;
	}
}

void Camera::calculateAngleAroundThePlayer() {
	if(inputManager.isLeftMouseButtonDown()) {
		float angleChange = display.getMouseXOffset() * 0.1f;
		m_angleAroundThePlayer -= angleChange;
	}
}

float Camera::calculateHorizontalDistance() {
	return m_distanceFromThePlayer * (float) glm::cos(glm::radians(m_pitch));
}

float Camera::calculateVerticalDistance() {
	return m_distanceFromThePlayer * (float)glm::sin(glm::radians(m_pitch));
}

void Camera::calculateCameraPosition(float horizontalDistance, float verticalDistance) {
	float theta = m_player.getRotY() + m_angleAroundThePlayer;
	float offsetX = (float)(horizontalDistance * glm::sin(glm::radians(theta)));
	float offsetZ = (float)(horizontalDistance * glm::cos(glm::radians(theta)));
	glm::vec3 playerPosition = m_player.getPosition();
	m_position.x = playerPosition.x - offsetX;
	m_position.y = playerPosition.y + verticalDistance;
	m_position.z = playerPosition.z - offsetZ;
}

void Camera::updateCameraVectors() {
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(m_front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}