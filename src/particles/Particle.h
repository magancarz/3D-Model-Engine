#pragma once

#include "../Headers.h"
#include "../renderEngine/DisplayManager.h"
#include "../entities/Camera.h"
#include "ParticleTexture.h"

class Particle {
public:
	Particle(ParticleTexture* texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);

	inline ParticleTexture* getTexture() { return m_texture; }
	inline glm::vec2 getTexOffset1() { return m_texOffset1; }
	inline glm::vec2 getTexOffset2() { return m_texOffset2; }
	inline float getBlend() { return m_blend; }
	inline glm::vec3 getPosition() { return m_position; }
	inline float getRotation() { return m_rotation; }
	inline float getScale() { return m_scale; }
	inline float getDistance() { return m_distance; }

	bool update(Camera* camera);

private:
	void updateTextureCoordInfo();
	void setTextureOffset(glm::vec2& offset, int index);

	ParticleTexture* m_texture;

	glm::vec2 m_texOffset1,
			  m_texOffset2;

	float m_blend;

	glm::vec3 m_position,
			  m_velocity;

	float m_gravityEffect,
		  m_lifeLength,
		  m_rotation,
		  m_scale;

	float m_elapsedTime = 0.0f,
		  m_distance;

};