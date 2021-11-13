#include "ParticleSystem.h"
#include "../renderEngine/DisplayManager.h"
#include "Particle.h"

ParticleSystem::ParticleSystem(ParticleTexture* texture, float pps, float speed, float gravityComplient, float lifeLength)
	: m_texture(texture), m_pps(pps), m_speed(speed), m_gravityComplient(gravityComplient), m_lifeLength(lifeLength) {
	
}

void ParticleSystem::generateParticles(glm::vec3 systemCenter) {
	GLfloat delta = display.getFrameTimeSeconds();
	GLfloat particlesToCreate = m_pps * delta;
	int count = floor(particlesToCreate);
	GLfloat partialParticle = particlesToCreate - (GLfloat)count;
	for (int i = 0; i < count; i++) {
		emitParticle(systemCenter);
	}
	if (rand() < partialParticle) {
		emitParticle(systemCenter);
	}
}

void ParticleSystem::emitParticle(glm::vec3 center) {
	GLfloat dirX = rand() * 2.0 - 1.0;
	GLfloat dirZ = rand() * 2.0 - 1.0;
	glm::vec3 velocity(dirX, 1, dirZ);
	velocity = glm::normalize(velocity);
	velocity *= glm::vec3(m_speed);

	// adds particle to the particle master
	Particle* particle = new Particle(m_texture, glm::vec3(center), velocity, m_gravityComplient, m_lifeLength, 0, 1);
}