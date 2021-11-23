#include "Particle.h"
#include "ParticleMaster.h"

extern ParticleMaster* particleMaster;

Particle::Particle(ParticleTexture* texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale) 
	: m_texture(texture), m_position(position), m_velocity(velocity), m_gravityEffect(gravityEffect), m_lifeLength(lifeLength), m_rotation(rotation), m_scale(scale) {
	particleMaster->addParticle(this);
}

bool Particle::update(Camera* camera) {
	m_velocity.y += GRAVITY * m_gravityEffect * display.getFrameTimeSeconds();
	glm::vec3 change(m_velocity);
	change *= display.getFrameTimeSeconds();
	m_position += change;
	updateTextureCoordInfo();
	m_elapsedTime += display.getFrameTimeSeconds();
	return m_elapsedTime < m_lifeLength;
}

void Particle::updateTextureCoordInfo() {
	float lifeFactor = m_elapsedTime / m_lifeLength;
	int stageCount = m_texture->getNumberOfRows() * m_texture->getNumberOfRows();
	float atlasProgression = lifeFactor * stageCount;
	int index1 = glm::floor(atlasProgression);
	int index2 = index1 < stageCount - 1? index1 + 1 : index1;
	m_blend = fmod(atlasProgression, 1);
	setTextureOffset(m_texOffset1, index1);
	setTextureOffset(m_texOffset2, index2);
}

void Particle::setTextureOffset(glm::vec2& offset, int index) {
	int column = index % m_texture->getNumberOfRows();
	int row = index / m_texture->getNumberOfRows();
	offset.x = (float)column / m_texture->getNumberOfRows();
	offset.y = (float)row / m_texture->getNumberOfRows();
}