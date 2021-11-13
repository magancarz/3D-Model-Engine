#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer(Loader* loader, glm::mat4 projectionMatrix) {
	std::vector<float> vertices = {
		-0.5f, 0.5f, -0.5f, -0.5f,
		 0.5f, 0.5f,  0.5f, -0.5f
	};
	quad = loader->loadToVAO(vertices, 2);

	m_shader = new ParticleShader();
	m_shader->start();
	m_shader->loadProjectionMatrix(projectionMatrix);
	m_shader->stop();
}

ParticleRenderer::~ParticleRenderer() {
	m_shader->cleanUp();
	delete m_shader;
}

void ParticleRenderer::render(std::map<ParticleTexture*, std::vector<Particle*>*>* particlesMap, Camera* camera) {
	glm::mat4 viewMatrix = camera->getView();
	prepare();
	for (auto mit = particlesMap->begin();  mit != particlesMap->end(); mit++) {
		ParticleTexture* texture = mit->first;
		std::vector<Particle*>* particles = mit->second;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
		//pointer = 0;
		//std::vector<GLfloat> vboData(particles->size() * INSTANCE_DATA_LENGTH);

		for (auto vit = particles->begin(); vit != particles->end(); vit++) {
			Particle* particle = *vit;

			updateModelViewMatrix(particle->getPosition(), particle->getRotation(),
					      particle->getScale(), viewMatrix);
			m_shader->loadTextureCoordInfo(particle->getTexOffset1(), particle->getTexOffset2(), texture->getNumberOfRows(), particle->getBlend());
		}
		//loader->updateVbo(vboID, vboData);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), particles->size());

	}
	finishRendering();
}

void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix) {
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	// set the rotation 3x3 part of the model matrix to the
	// transpose of the 3x3 rotation part of the view matrix
	modelMatrix[0][0] = viewMatrix[0][0];
	modelMatrix[0][1] = viewMatrix[1][0];
	modelMatrix[0][2] = viewMatrix[2][0];
	modelMatrix[1][0] = viewMatrix[0][1];
	modelMatrix[1][1] = viewMatrix[1][1];
	modelMatrix[1][2] = viewMatrix[2][1];
	modelMatrix[2][0] = viewMatrix[0][2];
	modelMatrix[2][1] = viewMatrix[1][2];
	modelMatrix[2][2] = viewMatrix[2][2];
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	m_shader->loadModelViewMatrix(modelViewMatrix);
	//storeMatrixData(modelViewMatrix, vboData);
}

void ParticleRenderer::prepare() {
	m_shader->start();
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
}

void ParticleRenderer::finishRendering() {
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_shader->stop();
}