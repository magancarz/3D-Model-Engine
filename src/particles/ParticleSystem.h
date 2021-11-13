#pragma once

#include "../Headers.h"
#include "ParticleTexture.h"

class ParticleSystem {
public:
	ParticleSystem(ParticleTexture* texture, float pps, float speed, float gravityComplient, float lifeLength);
	void generateParticles(glm::vec3 systemCenter);
	void emitParticle(glm::vec3 center);

private:
	ParticleTexture* m_texture;

	float m_pps,
		  m_speed,
		  m_gravityComplient,
		  m_lifeLength;

};