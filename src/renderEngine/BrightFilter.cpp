#include "BrightFilter.h"

BrightFilter::BrightFilter(unsigned int width, unsigned int height) :
m_shader(new BrightFilterShader), m_renderer(new ImageRenderer(width, height)) {
	
}
	
BrightFilter::~BrightFilter() {
	delete m_renderer;
	delete m_shader;
}

void BrightFilter::render(unsigned int texture) {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->renderQuad();
	m_shader->stop();
}