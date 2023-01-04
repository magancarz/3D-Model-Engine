#include "HorizontalBlur.h"

#include <GL/glew.h>

HorizontalBlur::HorizontalBlur(const unsigned int target_fbo_width, const unsigned int target_fbo_height) {
	m_shader = std::make_unique<HorizontalBlurShader>();
	m_renderer = std::make_unique<ImageRenderer>(target_fbo_width, target_fbo_height);
	m_shader->start();
	m_shader->load_target_width(target_fbo_width);
	m_shader->stop();
}

void HorizontalBlur::render(const unsigned int texture) const {
	m_shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_renderer->render_quad();
	m_shader->stop();
}

unsigned int HorizontalBlur::get_output_texture() const { return m_renderer->get_output_texture(); }