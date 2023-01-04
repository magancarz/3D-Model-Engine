#include "BrightFilter.h"

#include <GL/glew.h>

BrightFilter::BrightFilter(const unsigned int width, const unsigned int height) {
	m_shader = std::make_unique<BrightFilterShader>();
	m_renderer = std::make_unique<ImageRenderer>();
}

void BrightFilter::render(const unsigned int texture) const {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->render_quad();
	m_shader->stop();
}

unsigned int BrightFilter::get_output_texture() const {
	return m_renderer->get_output_texture();
}