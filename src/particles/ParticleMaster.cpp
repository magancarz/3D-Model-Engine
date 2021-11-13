#include "ParticleMaster.h"

ParticleMaster::ParticleMaster(Loader* loader, glm::mat4 projectionMatrix) {
	m_renderer = new ParticleRenderer(loader, projectionMatrix);
}

ParticleMaster::~ParticleMaster() {
	delete m_renderer;
	delete m_particles;
}

void ParticleMaster::update(Camera* camera) {
	auto mit = m_particles->begin();
	while (mit != m_particles->end()) {
		ParticleTexture* texture = mit->first;
		std::vector<Particle*>* particles = mit->second;
		auto vit = particles->begin();
		while (vit != particles->end()) {
			Particle* p = *vit;
			bool stillAlive = p->update(camera);
			if (!stillAlive) {
				vit = particles->erase(vit);
			} else {
				vit++;
			}
		}

		//if (!texture->isAdditive()) {
			// In the case of alpha blended particles, we want to paint
			// the particle that is furthest away first.
			// uses operator <
		//	std::sort(particles.begin(), particles.end());
		//}
		mit++;
	}
}

void ParticleMaster::renderParticles(Camera* camera) {
	m_renderer->render(m_particles, camera);
}

void ParticleMaster::addParticle(Particle* particle) {
	ParticleTexture* texture = particle->getTexture();
	auto it = m_particles->find(texture);
	if(it != m_particles->end()) {
		std::vector<Particle*>* particles = it->second;
		particles->push_back(particle);
	} else {
		std::vector<Particle*>* particles = new std::vector<Particle*>;
		particles->push_back(particle);
		m_particles->insert(std::make_pair(texture, particles));
	}
}