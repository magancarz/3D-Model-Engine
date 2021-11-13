#pragma once

#include "../Headers.h"

class ParticleSystem {
public:
	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength);
	void generateParticles(glm::vec3 systemCenter);
	void emitParticle(glm::vec3 center);

private:
	float m_pps,
		  m_speed,
		  m_gravityComplient,
		  m_lifeLength;

};