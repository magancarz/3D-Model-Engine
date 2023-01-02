#pragma once

#include <glm/glm.hpp>

class Light {
public:
	Light(const glm::vec3& position, const glm::vec3& color, const glm::vec3& attenuation);

	void set_position(const glm::vec3& position);
	void set_color(const glm::vec3& color);
	void set_attenuation(const glm::vec3& attenuation);

	glm::vec3 get_position() const;
	glm::vec3 get_color() const;
	glm::vec3 get_attenuation() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_color;
	glm::vec3 m_attenuation;
};