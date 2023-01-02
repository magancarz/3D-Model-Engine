#include "VerticalBlur.h"

#include <GL/glew.h>

VerticalBlur::VerticalBlur(unsigned int targetFBOWidth, unsigned int targetFBOHeight) :
m_shader(new VerticalBlurShader), m_renderer(new ImageRenderer(targetFBOWidth, targetFBOHeight)) {
	m_shader->start();
	m_shader->loadTargetHeight(targetFBOHeight);
	m_shader->stop();
}

void VerticalBlur::render(unsigned int texture) {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->renderQuad();
	m_shader->stop();
}

void VerticalBlur::cleanUp() {
	m_renderer->cleanUp();
	delete m_renderer;
	delete m_shader;
}