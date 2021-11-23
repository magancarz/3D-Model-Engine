#pragma once

#include "../Headers.h"
#include "ParticleTexture.h"

class ParticleSystem {
public:
	ParticleSystem(ParticleTexture* texture, float pps, float speed, float gravityComplient, float lifeLength, float scale);
	
	void setDirection(glm::vec3 direction, float deviation) {
		m_direction = glm::vec3(direction);
		m_directionDeviation = (float) (deviation * glm::pi<float>());
	}

	void randomizeRotation() { m_randomRotation = true; }

	void setSpeedError(float error) { m_speedError = m_averageSpeed * error; }
	void setLifeError(float error) { m_lifeError = m_averageLifeLength * error; }
	void setScaleError(float error) { m_scaleError = m_averageScale * error; }

	void generateParticles(glm::vec3 systemCenter);
	void emitParticle(glm::vec3 center);

private:
	float generateValue(float average, float errorMargin);
	float generateRotation();

	glm::vec3 generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle);
	glm::vec3 generateRandomUnitVector();

	ParticleTexture* m_texture;

	float m_pps,
		  m_averageSpeed,
		  m_gravityComplient,
		  m_averageLifeLength,
		  m_averageScale;

	float m_speedError,
		  m_lifeError,
		  m_scaleError;

	bool m_randomRotation;

	glm::vec3 m_direction;

	float m_directionDeviation = -1.0f;

};