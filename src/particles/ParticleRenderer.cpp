#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer(Loader* loader, glm::mat4 projectionMatrix)
	: m_loader(loader) {
	m_buffer.resize(INSTANCE_DATA_LENGTH * MAX_INSTANCES);
	m_vboID = m_loader->createEmptyVBO(m_buffer);
	
	std::vector<float> vertices = {
		-0.5f, 0.5f, -0.5f, -0.5f,
		 0.5f, 0.5f,  0.5f, -0.5f
	};
	quad = loader->loadToVAO(vertices, 2);

	m_loader->addInstanceAttribute(quad->getVaoID(), m_vboID, 1, 4, INSTANCE_DATA_LENGTH, 0);
	m_loader->addInstanceAttribute(quad->getVaoID(), m_vboID, 2, 4, INSTANCE_DATA_LENGTH, 4);
	m_loader->addInstanceAttribute(quad->getVaoID(), m_vboID, 3, 4, INSTANCE_DATA_LENGTH, 8);
	m_loader->addInstanceAttribute(quad->getVaoID(), m_vboID, 4, 4, INSTANCE_DATA_LENGTH, 12);
	m_loader->addInstanceAttribute(quad->getVaoID(), m_vboID, 5, 4, INSTANCE_DATA_LENGTH, 16);
	m_loader->addInstanceAttribute(quad->getVaoID(), m_vboID, 6, 1, INSTANCE_DATA_LENGTH, 20);

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

	for(std::map<ParticleTexture*, std::vector<Particle*>*>::iterator pit = particlesMap->begin();  pit != particlesMap->end(); pit++) {
		std::vector<Particle*>* particles = pit->second;
		ParticleTexture* texture = pit->first;
		bindTexture(texture);
		m_pointer = 0;
		std::vector<float> vboData(particles->size() * INSTANCE_DATA_LENGTH);
		
		for(std::vector<Particle*>::iterator it = particles->begin(); it != particles->end(); it++) {
			Particle* particle = *it;

			updateModelViewMatrix(particle->getPosition(), particle->getRotation(),
					      particle->getScale(), viewMatrix, vboData);
			updateTexCoordInfo(particle, vboData);
		}
		m_loader->updateVBO(m_vboID, vboData);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), particles->size());
	}
	finishRendering();
}

void ParticleRenderer::bindTexture(ParticleTexture* texture) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	m_shader->loadNumberOfRows(texture->getNumberOfRows());
}

void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix, std::vector<float>& vboData) {
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
	storeMatrixData(modelViewMatrix, vboData);
}

void ParticleRenderer::storeMatrixData(glm::mat4 matrix, std::vector<float>& vboData) {
	vboData[m_pointer++] = matrix[0][0];
	vboData[m_pointer++] = matrix[0][1];
	vboData[m_pointer++] = matrix[0][2];
	vboData[m_pointer++] = matrix[0][3];
	vboData[m_pointer++] = matrix[1][0];
	vboData[m_pointer++] = matrix[1][1];
	vboData[m_pointer++] = matrix[1][2];
	vboData[m_pointer++] = matrix[1][3];
	vboData[m_pointer++] = matrix[2][0];
	vboData[m_pointer++] = matrix[2][1];
	vboData[m_pointer++] = matrix[2][2];
	vboData[m_pointer++] = matrix[2][3];
	vboData[m_pointer++] = matrix[3][0];
	vboData[m_pointer++] = matrix[3][1];
	vboData[m_pointer++] = matrix[3][2];
	vboData[m_pointer++] = matrix[3][3];
}

void ParticleRenderer::updateTexCoordInfo(Particle* particle, std::vector<float>& data) {
	data[m_pointer++] = particle->getTexOffset1().x;
	data[m_pointer++] = particle->getTexOffset1().y;
	data[m_pointer++] = particle->getTexOffset2().x;
	data[m_pointer++] = particle->getTexOffset2().y;
	data[m_pointer++] = particle->getBlend();
}

void ParticleRenderer::prepare() {
	m_shader->start();
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);
}

void ParticleRenderer::finishRendering() {
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glBindVertexArray(0);
	m_shader->stop();
}