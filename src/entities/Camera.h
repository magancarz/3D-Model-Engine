#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
	Camera();

	void move();

	inline glm::vec3 getPosition() const { return m_position; };
	inline float getPitch() const { return m_pitch; };
	inline float getYaw() const { return m_yaw; };
	inline float getRoll() const { return m_roll; };

private:
	glm::vec3 m_position;
	float m_pitch;
	float m_yaw;
	float m_roll;
};