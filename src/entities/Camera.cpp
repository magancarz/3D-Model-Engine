#include "Camera.h"

Camera::Camera(glm::vec3 position)
	: m_position(position) {
	m_movementSpeed = 0.3f;
	m_sensitivity = 0.35f;

	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(0.0f);
	m_up = m_worldUp;

	m_pitch = 0.0f;
	m_yaw = -90.0f;
	m_roll = 0.0f;

	updateCameraVectors();
}

void Camera::move() {
	updateCameraVectors();

	if(inputManager.isKeyDown(GLFW_KEY_W))
		m_position += m_front * m_movementSpeed;
	else if(inputManager.isKeyDown(GLFW_KEY_S))
		m_position -= m_front * m_movementSpeed;
	if(inputManager.isKeyDown(GLFW_KEY_A))
		m_position -= m_right * m_movementSpeed;
	else if(inputManager.isKeyDown(GLFW_KEY_D))
		m_position += m_right * m_movementSpeed;
}

void Camera::updateCameraVectors() {
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(m_front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::rotate(float pitch, float yaw, float roll) {
	m_pitch += static_cast<float>(yaw) * m_sensitivity;
	m_yaw += static_cast<float>(pitch) * m_sensitivity;

	if(this->m_pitch > 80.f)
		this->m_pitch = 80.f;
	else if(this->m_pitch < -80.f)
		this->m_pitch = -80.f;

	if(this->m_yaw > 360.f || this->m_yaw < -360.f)
		this->m_yaw = 0.f;
}

glm::mat4 Camera::getView() {
	return glm::lookAt(
		m_position,
		m_position + m_front,
		m_up
	);
}