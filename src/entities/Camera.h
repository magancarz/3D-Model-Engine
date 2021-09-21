#pragma once

#include "../Headers.h"

#include "../toolbox/Input.h"

extern Input inputManager;

class Camera {
public:
	Camera();

	void move();

	void setPosition(glm::vec3 position) { m_position = position; };

	glm::mat4 getView();

	inline glm::vec3 getPosition() const { return m_position; };
	inline float getRotX() const { return m_rx; };
	inline float getRotY() const { return m_ry; };
	inline float getRotZ() const { return m_rz; };

private:
	glm::vec3 m_position;
	float m_rx;
	float m_ry;
	float m_rz;
};