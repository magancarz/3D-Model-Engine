#include "VerticalBlur.h"

#include <GL/glew.h>

VerticalBlur::VerticalBlur(const unsigned int target_fbo_width, const unsigned int target_fbo_height) {
	m_shader = std::make_unique<VerticalBlurShader>();
	m_renderer = std::make_unique<ImageRenderer>(target_fbo_width, target_fbo_height);

	m_shader->start();
	m_shader->load_target_height(target_fbo_height);
	m_shader->stop();
}

void VerticalBlur::render(const unsigned int texture) const {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->render_quad();
	m_shader->stop();
}

unsigned int VerticalBlur::get_output_texture() const { return m_renderer->get_output_texture(); }