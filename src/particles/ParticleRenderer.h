#pragma once

#include "../Headers.h"
#include "../models/RawModel.h"
#include "../renderEngine/Loader.h"
#include "../entities/Camera.h"
#include "ParticleShader.h"
#include "Particle.h"

class ParticleRenderer {
public:
	ParticleRenderer(Loader* loader, glm::mat4 projectionMatrix);
	~ParticleRenderer();
	void render(std::vector<Particle*>* particles, Camera* camera);
	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix);
	void prepare();
	void finishRendering();

private:
	RawModel* quad;
	ParticleShader* m_shader;

};