#pragma once

#include "../Headers.h"

#include "../toolbox/Input.h"

extern Input inputManager;

enum direction {FORWARD, BACKWARD, LEFT, RIGHT};

class Camera {
public:
	Camera(glm::vec3 position);

	void move();
	void updateCameraVectors();
	void rotate(float pitch, float yaw, float roll);

	void setPosition(glm::vec3 position) { m_position = position; };
	void setPitch(float pitch) { m_pitch = pitch; };
	void setYaw(float yaw) { m_yaw = yaw; };
	void setRoll(float roll) { m_roll = roll; };

	void increasePitch(float pitch) { m_pitch += pitch * m_sensitivity; };
	void increaseYaw(float yaw) { m_yaw += yaw * m_sensitivity; };
	void increaseRoll(float roll) { m_roll += roll * m_sensitivity; };

	glm::mat4 getView();

	inline glm::vec3 getPosition() const { return m_position; };
	inline float getPitch() const { return m_pitch; };
	inline float getYaw() const { return m_yaw; };
	inline float getRoll() const { return m_roll; };
	inline float getSensitivity() const { return m_sensitivity; };

private:
	glm::vec3 m_position;
	glm::vec3 m_worldUp, m_front, m_right, m_up;

	float m_movementSpeed, m_sensitivity;
	float m_pitch, m_yaw, m_roll;
};