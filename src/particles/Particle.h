#pragma once

#include "../Headers.h"
#include "../renderEngine/DisplayManager.h"

class Particle {
public:
	Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);

	inline glm::vec3 getPosition() { return m_position; }
	inline float getRotation() { return m_rotation; }
	inline float getScale() { return m_scale; }

	bool update();

private:
	glm::vec3 m_position,
			  m_velocity;

	float m_gravityEffect,
		  m_lifeLength,
		  m_rotation,
		  m_scale;

	float m_elapsedTime = 0.0f;

};