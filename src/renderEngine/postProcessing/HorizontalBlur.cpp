#include "HorizontalBlur.h"

#include <GL/glew.h>

HorizontalBlur::HorizontalBlur(unsigned int targetFBOWidth, unsigned int targetFBOHeight) {
	m_shader = new HorizontalBlurShader();
	m_renderer = new ImageRenderer(targetFBOWidth, targetFBOHeight);
	m_shader->start();
	m_shader->loadTargetWidth(targetFBOWidth);
	m_shader->stop();
}

void HorizontalBlur::render(unsigned int texture) {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->renderQuad();
	m_shader->stop();
}

void HorizontalBlur::cleanUp() {
	m_renderer->cleanUp();
	delete m_renderer;
	m_shader->cleanUp();
	delete m_shader;
}