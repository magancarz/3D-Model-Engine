#include "Camera.h"

Camera::Camera()
	: m_position(glm::vec3(0.0f, 0.0f, 1.0f)), m_pitch(0), m_yaw(0), m_roll(0) {

}

void Camera::move() {
	/*
	if(glfwGetKey(WINDOW, GLFW_KEY_W) == GLFW_PRESS)
		m_position.z -= 0.02f;
	if(glfwGetKey(WINDOW, GLFW_KEY_D) == GLFW_PRESS)
		m_position.x += 0.02f;
	if(glfwGetKey(WINDOW, GLFW_KEY_A) == GLFW_PRESS)
		m_position.x -= 0.02f;
	*/
}

