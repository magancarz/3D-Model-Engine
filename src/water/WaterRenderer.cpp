#include "WaterRenderer.h"

WaterRenderer::WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projection, WaterFrameBuffers* fbos)
	: m_shader(shader), m_fbos(fbos) {
	m_shader->start();
	m_shader->connectTextureUnits();
	m_shader->loadProjectionMatrix(projection);
	m_shader->stop();

	m_dudvTexture = loader->loadTexture("res/textures/waterDUDV.png");

	setUpVAO(*loader);
}

void WaterRenderer::render(std::vector<WaterTile*>& water, Camera& camera) {
	prepareRender(camera);
	for(std::vector<WaterTile*>::iterator it = water.begin(); it < water.end(); it++) {
		glm::mat4 modelMatrix = createTransformationMatrix(glm::vec3((*it)->getX(), (*it)->getHeight(), (*it)->getZ()), 0, 0, 0, WATER_TILE_SIZE);
		m_shader->loadModelMatrix(modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, quad->getVertexCount());
	}
	unbind();
}

void WaterRenderer::prepareRender(Camera& camera) {
	m_shader->start();
	m_shader->loadViewMatrix(camera);
	moveFactor += WAVE_SPEED * display.getFrameTimeSeconds();
	moveFactor = fmod(moveFactor, 1.0);
	m_shader->loadMoveFactor(moveFactor);
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbos->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fbos->getRefractionTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_dudvTexture);
}

void WaterRenderer::unbind() {
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_shader->stop();
}

void WaterRenderer::setUpVAO(Loader& loader) {
	//Just x and z vertex positions here, y is set to 0 in v.shader
	std::vector<float> vertices = {
		-1, -1, -1,
		 1,  1, -1,
		 1, -1, -1,
		 1,  1,  1
	};
	quad = loader.loadToVAO(vertices, 2);
}