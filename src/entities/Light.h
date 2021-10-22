#pragma once

#include "../Headers.h"

class Light {
public:
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation);

	void setPosition(glm::vec3 position) { m_position = position; };
	void setColor(glm::vec3 color) { m_color = color; };
	void setAttenuation(glm::vec3 attenuation) { m_attenuation = attenuation; };

	inline glm::vec3 getPosition() { return m_position; };
	inline glm::vec3 getColor() { return m_color; };
	inline glm::vec3 getAttenuation() { return m_attenuation; };

private:
	glm::vec3 m_position;
	glm::vec3 m_color;
	glm::vec3 m_attenuation;
};