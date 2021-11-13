#include "ParticleMaster.h"

ParticleMaster::ParticleMaster(Loader* loader, glm::mat4 projectionMatrix) {
	m_renderer = new ParticleRenderer(loader, projectionMatrix);
}

ParticleMaster::~ParticleMaster() {
	delete m_renderer;
	delete m_particles;
}

void ParticleMaster::update() {
	auto vit = m_particles->begin();
	while(vit != m_particles->end()) {
		Particle* p = *vit;
		bool stillAlive = p->update();
		if(!stillAlive) {
			vit = m_particles->erase(vit);
		} else {
			vit++;
		}
	}
}

void ParticleMaster::renderParticles(Camera* camera) {
	m_renderer->render(m_particles, camera);
}

void ParticleMaster::addParticle(Particle* particle) {
	m_particles->push_back(particle);
}