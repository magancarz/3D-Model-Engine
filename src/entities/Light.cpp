#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation)
	: m_position(position), m_color(color), m_attenuation(attenuation) {}