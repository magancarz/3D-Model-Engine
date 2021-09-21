#include "Camera.h"

Camera::Camera()
	: m_position(glm::vec3(0.0f, 0.0f, 0.0f)), m_rx(0), m_ry(0), m_rz(0) {}

void Camera::move() {
	if(inputManager.isKeyDown(GLFW_KEY_W))
		m_position.z += 0.2f;
	else if(inputManager.isKeyDown(GLFW_KEY_S))
		m_position.z -= 0.2f;
	if(inputManager.isKeyDown(GLFW_KEY_A))
		m_position.x += 0.2f;
	else if(inputManager.isKeyDown(GLFW_KEY_D))
		m_position.x -= 0.2f;
	if(inputManager.isKeyDown(GLFW_KEY_SPACE))
		m_position.y += 0.2f;
	else if(inputManager.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		m_position.y -= 0.2f;
	if(inputManager.isKeyDown(GLFW_KEY_E))
		m_ry += 0.2f;
	else if(inputManager.isKeyDown(GLFW_KEY_Q))
		m_ry -= 0.2f;
}

glm::mat4 Camera::getView() {
	return glm::lookAt(
		m_position,
		m_position + glm::vec3(0,0,1),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
}