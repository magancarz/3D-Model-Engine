#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(Loader* loader, glm::mat4 projectionMatrix) {
	m_cube = loader->loadToVAO(VERTICES, 3);
	m_texture = loader->loadCubeMap(TEXTURE_FILES);
	m_nightTexture = loader->loadCubeMap(NIGHT_TEXTURE_FILES);
	
	m_shader = new SkyboxShader();
	m_shader->start();
	m_shader->connectTextureUnits();
	m_shader->loadProjectionMatrix(projectionMatrix);
	m_shader->stop();
}

SkyboxRenderer::~SkyboxRenderer() {
	//Clean up
	delete m_cube;
	delete m_shader;
}

void SkyboxRenderer::render(Camera& camera, float r, float g, float b) {
	m_shader->start();
	m_shader->loadViewMatrix(camera);
	m_shader->loadFogColor(r, g, b);
	glBindVertexArray(m_cube->getVaoID());
	glEnableVertexAttribArray(0);
	bindTextures();
	glDrawArrays(GL_TRIANGLES, 0, m_cube->getVertexCount());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_shader->stop();
}

void SkyboxRenderer::bindTextures() {
	//time += display.getFrameTimeSeconds() * 100; // original: * 1000
	time = 1000;
	while (time >= 24000) {
		time -= 24000;
	}
	int texture1, texture2;
	GLfloat blendFactor;
	if (time < 5000) {
		texture1 = m_nightTexture;
		texture2 = m_nightTexture;
		blendFactor = (time - 0) / (5000 - 0);
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = m_nightTexture;
		texture2 = m_texture;
		blendFactor = (time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = m_texture;
		texture2 = m_texture;
		blendFactor = (time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = m_texture;
		texture2 = m_nightTexture;
		blendFactor = (time - 21000) / (24000 - 21000);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
	m_shader->loadBlendFactor(blendFactor);
}