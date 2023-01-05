#include "CombineFilter.h"

#include <GL/glew.h>

CombineFilter::CombineFilter() {
	m_shader = std::make_unique<CombineShader>();
	m_shader->start();
	m_shader->bind_attributes();
	m_shader->get_all_uniform_locations();
	m_shader->connect_texture_units();
	m_shader->stop();

	m_renderer = std::make_unique<ImageRenderer>();
}

void CombineFilter::render(const unsigned int color_texture, const unsigned int highlight_texture) const {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, highlight_texture);
	m_renderer->render_quad();
	m_shader->stop();
}