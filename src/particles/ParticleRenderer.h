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
	void render(std::map<ParticleTexture*, std::vector<Particle*>*>* particles, Camera* camera);
	void bindTexture(ParticleTexture* texture);
	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix, std::vector<float>& vboData);
	void storeMatrixData(glm::mat4 matrix, std::vector<float>& vboData);
	void updateTexCoordInfo(Particle* particle, std::vector<float>& data);
	void prepare();
	void finishRendering();

private:
	const unsigned int MAX_INSTANCES = 10000,
					   INSTANCE_DATA_LENGTH = 21;

	Loader* m_loader;
	unsigned int m_vboID;
	std::vector<float> m_buffer;
	unsigned int m_pointer = 0;
	
	RawModel* quad;
	ParticleShader* m_shader;

};