#include "Particle.h"
#include "ParticleMaster.h"

extern ParticleMaster* particleMaster;

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale) 
	: m_position(position), m_velocity(velocity), m_gravityEffect(gravityEffect), m_lifeLength(lifeLength), m_rotation(rotation), m_scale(scale) {
	particleMaster->addParticle(this);
}

bool Particle::update() {
	m_velocity.y += GRAVITY * m_gravityEffect * display.getFrameTimeSeconds();
	glm::vec3 change(m_velocity);
	change *= display.getFrameTimeSeconds();
	m_position += change;
	m_elapsedTime += display.getFrameTimeSeconds();
	return m_elapsedTime < m_lifeLength;
}