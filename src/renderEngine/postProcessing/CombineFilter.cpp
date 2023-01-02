#include "CombineFilter.h"

#include <GL/glew.h>

CombineFilter::CombineFilter() {
	m_shader = new CombineShader();
	m_shader->start();
	m_shader->connectTextureUnits();
	m_shader->stop();

	m_renderer = new ImageRenderer();
}

CombineFilter::~CombineFilter() {
	delete m_shader;
	delete m_renderer;
}

void CombineFilter::render(unsigned int colorTexture, unsigned int highlightTexture) {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, highlightTexture);
	m_renderer->renderQuad();
	m_shader->stop();
}