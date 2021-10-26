#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(Loader* loader, glm::mat4 projectionMatrix) {
	m_cube = loader->loadToVAO(VERTICES, 3);
	m_texture = loader->loadCubeMap(TEXTURE_FILES);
	
	m_shader = new SkyboxShader();
	m_shader->start();
	m_shader->loadProjectionMatrix(projectionMatrix);
	m_shader->stop();
}

SkyboxRenderer::~SkyboxRenderer() {
	//Clean up
	delete m_cube;
	delete m_shader;
}

void SkyboxRenderer::render(Camera& camera) {
	m_shader->start();
	m_shader->loadViewMatrix(camera);
	glBindVertexArray(m_cube->getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glDrawArrays(GL_TRIANGLES, 0, m_cube->getVertexCount());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_shader->stop();
}