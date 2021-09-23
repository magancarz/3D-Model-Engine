#pragma once

#include "../Headers.h"

class Light {
public:
	Light(glm::vec3 position, glm::vec3 color);

	void setPosition(glm::vec3 position) { m_position = position; };
	void setColor(glm::vec3 color) { m_color = color; };

	inline glm::vec3 getPosition() { return m_position; };
	inline glm::vec3 getColor() { return m_color; };

private:
	glm::vec3 m_position;
	glm::vec3 m_color;
};