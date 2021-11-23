#include "ParticleSystem.h"
#include "../renderEngine/DisplayManager.h"
#include "Particle.h"

ParticleSystem::ParticleSystem(ParticleTexture* texture, float pps, float speed, float gravityComplient, float lifeLength, float scale)
	: m_texture(texture), m_pps(pps), m_averageSpeed(speed), m_gravityComplient(gravityComplient), m_averageLifeLength(lifeLength), m_averageScale(scale) {
	
}

void ParticleSystem::generateParticles(glm::vec3 systemCenter) {
	float delta = display.getFrameTimeSeconds();
	float particlesToCreate = m_pps * delta;
	int count = floor(particlesToCreate);
	float partialParticle = particlesToCreate - (float)count;
	for (int i = 0; i < count; i++) {
		emitParticle(systemCenter);
	}
	if (random() < partialParticle) {
		emitParticle(systemCenter);
	}
}

void ParticleSystem::emitParticle(glm::vec3 center) {
	//GLfloat dirX = rand() * 2.0 - 1.0;
	//GLfloat dirZ = rand() * 2.0 - 1.0;
	//glm::vec3 velocity(dirX, 1, dirZ);
	glm::vec3 velocity = glm::vec3(0);
	if(m_directionDeviation != -1) {
		velocity = generateRandomUnitVectorWithinCone(m_direction, m_directionDeviation);
	} else {
		velocity = generateRandomUnitVector();
	}
	velocity = glm::normalize(velocity);
	velocity *= generateValue(m_averageSpeed, m_speedError);
	float scale = generateValue(m_averageScale, m_scaleError);
	float lifeLength = generateValue(m_averageLifeLength, m_lifeError);
	Particle* particle = new Particle(m_texture, glm::vec3(center), velocity, m_gravityComplient, lifeLength, generateRotation(), scale);
}

float ParticleSystem::generateValue(float average, float errorMargin) {
	float offset = (random() - 0.5f) * 2.0f * errorMargin;
	return average + offset;
}

float ParticleSystem::generateRotation() {
	if(m_randomRotation) {
		return random() * 360.0f;
	} else {
		return 0;
	}
}

glm::vec3 ParticleSystem::generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle) {
	float cosAngle = glm::cos(angle);
	float theta = random() * 2.0f * glm::pi<float>();
	float z = cosAngle + (random() * (1 - cosAngle));
	float rootOneMinusZSquared = glm::sqrt(1 - z * z);
	float x = rootOneMinusZSquared * glm::cos(theta);
	float y = rootOneMinusZSquared * glm::sin(theta);

	glm::vec4 direction(x, y, z, 1);
	if(coneDirection.x == 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
		glm::vec3 rotateAxis = glm::cross(coneDirection, glm::vec3(0, 0, 1));
		rotateAxis = glm::normalize(rotateAxis);
		float rotateAngle = glm::acos(glm::dot(coneDirection, glm::vec3(0, 0, 1)));
		glm::mat4 rotationMatrix(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, -rotateAngle, rotateAxis);
		direction = rotationMatrix * direction;
	} else if(coneDirection.z == -1) {
		direction.z *= -1;
	}
	return glm::vec3(direction.x, direction.y, direction.z);
}

glm::vec3 ParticleSystem::generateRandomUnitVector() {
	float theta = random() * 2.0f * glm::pi<float>();
	float z = random() * 2.0f - 1.0f;
	float rootOneMinusZSquared = glm::sqrt(1.0f - z * z);
	float x = rootOneMinusZSquared * glm::cos(theta);
	float y = rootOneMinusZSquared * glm::sin(theta);
	return glm::vec3(x, y, z);
}