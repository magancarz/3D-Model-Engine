#pragma once

#include "../Headers.h"
#include "Particle.h"
#include "ParticleRenderer.h"

extern const float GRAVITY;

class ParticleMaster {
public:
	ParticleMaster(Loader* loader, glm::mat4 projectionMatrix);
	~ParticleMaster();
	void update(Camera* camera);
	void renderParticles(Camera* camera);
	void addParticle(Particle* particle);

private:
	std::map<ParticleTexture*, std::vector<Particle*>*>* m_particles = new std::map<ParticleTexture*, std::vector<Particle*>*>;
	ParticleRenderer* m_renderer;

};