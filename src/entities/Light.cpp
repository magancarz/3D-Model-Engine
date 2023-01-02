#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, const glm::vec3& attenuation) :
m_position(position), m_color(color), m_attenuation(attenuation) {}

void Light::set_position(const glm::vec3& position) { m_position = position; }
void Light::set_color(const glm::vec3& color) { m_color = color; }
void Light::set_attenuation(const glm::vec3& attenuation) { m_attenuation = attenuation; }

glm::vec3 Light::get_position() const { return m_position; }
glm::vec3 Light::get_color() const { return m_color; }
glm::vec3 Light::get_attenuation() const { return m_attenuation; }